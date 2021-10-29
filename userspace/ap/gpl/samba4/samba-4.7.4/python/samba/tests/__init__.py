# Unix SMB/CIFS implementation.
# Copyright (C) Jelmer Vernooij <jelmer@samba.org> 2007-2010
# Copyright (C) Stefan Metzmacher 2014,2015
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

"""Samba Python tests."""

import os
import ldb
import samba
from samba import param
from samba import credentials
from samba.credentials import Credentials
from samba import gensec
import socket
import struct
import subprocess
import sys
import tempfile
import unittest
import samba.auth
import samba.dcerpc.base
from samba.compat import PY3
if not PY3:
    # Py2 only
    from samba.samdb import SamDB
    import samba.ndr
    import samba.dcerpc.dcerpc
    import samba.dcerpc.epmapper

try:
    from unittest import SkipTest
except ImportError:
    class SkipTest(Exception):
        """Test skipped."""

HEXDUMP_FILTER=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])

class TestCase(unittest.TestCase):
    """A Samba test case."""

    def setUp(self):
        super(TestCase, self).setUp()
        test_debug_level = os.getenv("TEST_DEBUG_LEVEL")
        if test_debug_level is not None:
            test_debug_level = int(test_debug_level)
            self._old_debug_level = samba.get_debug_level()
            samba.set_debug_level(test_debug_level)
            self.addCleanup(samba.set_debug_level, test_debug_level)

    def get_loadparm(self):
        return env_loadparm()

    def get_credentials(self):
        return cmdline_credentials

    def hexdump(self, src):
        N = 0
        result = ''
        while src:
            ll = src[:8]
            lr = src[8:16]
            src = src[16:]
            hl = ' '.join(["%02X" % ord(x) for x in ll])
            hr = ' '.join(["%02X" % ord(x) for x in lr])
            ll = ll.translate(HEXDUMP_FILTER)
            lr = lr.translate(HEXDUMP_FILTER)
            result += "[%04X] %-*s  %-*s  %s %s\n" % (N, 8*3, hl, 8*3, hr, ll, lr)
            N += 16
        return result

    def insta_creds(self, template=None, username=None, userpass=None, kerberos_state=None):

        if template is None:
            assert template is not None

        if username is not None:
            assert userpass is not None

        if username is None:
            assert userpass is None

            username = template.get_username()
            userpass = template.get_password()

        if kerberos_state is None:
            kerberos_state = template.get_kerberos_state()

        # get a copy of the global creds or a the passed in creds
        c = Credentials()
        c.set_username(username)
        c.set_password(userpass)
        c.set_domain(template.get_domain())
        c.set_realm(template.get_realm())
        c.set_workstation(template.get_workstation())
        c.set_gensec_features(c.get_gensec_features()
                              | gensec.FEATURE_SEAL)
        c.set_kerberos_state(kerberos_state)
        return c



    # These functions didn't exist before Python2.7:
    if sys.version_info < (2, 7):
        import warnings

        def skipTest(self, reason):
            raise SkipTest(reason)

        def assertIn(self, member, container, msg=None):
            self.assertTrue(member in container, msg)

        def assertIs(self, a, b, msg=None):
            self.assertTrue(a is b, msg)

        def assertIsNot(self, a, b, msg=None):
            self.assertTrue(a is not b, msg)

        def assertIsNotNone(self, a, msg=None):
            self.assertTrue(a is not None)

        def assertIsInstance(self, a, b, msg=None):
            self.assertTrue(isinstance(a, b), msg)

        def assertIsNone(self, a, msg=None):
            self.assertTrue(a is None, msg)

        def assertGreater(self, a, b, msg=None):
            self.assertTrue(a > b, msg)

        def assertGreaterEqual(self, a, b, msg=None):
            self.assertTrue(a >= b, msg)

        def assertLess(self, a, b, msg=None):
            self.assertTrue(a < b, msg)

        def assertLessEqual(self, a, b, msg=None):
            self.assertTrue(a <= b, msg)

        def addCleanup(self, fn, *args, **kwargs):
            self._cleanups = getattr(self, "_cleanups", []) + [
                (fn, args, kwargs)]

        def _addSkip(self, result, reason):
            addSkip = getattr(result, 'addSkip', None)
            if addSkip is not None:
                addSkip(self, reason)
            else:
                warnings.warn("TestResult has no addSkip method, skips not reported",
                              RuntimeWarning, 2)
                result.addSuccess(self)

        def run(self, result=None):
            if result is None: result = self.defaultTestResult()
            result.startTest(self)
            testMethod = getattr(self, self._testMethodName)
            try:
                try:
                    self.setUp()
                except SkipTest as e:
                    self._addSkip(result, str(e))
                    return
                except KeyboardInterrupt:
                    raise
                except:
                    result.addError(self, self._exc_info())
                    return

                ok = False
                try:
                    testMethod()
                    ok = True
                except SkipTest as e:
                    self._addSkip(result, str(e))
                    return
                except self.failureException:
                    result.addFailure(self, self._exc_info())
                except KeyboardInterrupt:
                    raise
                except:
                    result.addError(self, self._exc_info())

                try:
                    self.tearDown()
                except SkipTest as e:
                    self._addSkip(result, str(e))
                except KeyboardInterrupt:
                    raise
                except:
                    result.addError(self, self._exc_info())
                    ok = False

                for (fn, args, kwargs) in reversed(getattr(self, "_cleanups", [])):
                    fn(*args, **kwargs)
                if ok: result.addSuccess(self)
            finally:
                result.stopTest(self)


class LdbTestCase(TestCase):
    """Trivial test case for running tests against a LDB."""

    def setUp(self):
        super(LdbTestCase, self).setUp()
        self.filename = os.tempnam()
        self.ldb = samba.Ldb(self.filename)

    def set_modules(self, modules=[]):
        """Change the modules for this Ldb."""
        m = ldb.Message()
        m.dn = ldb.Dn(self.ldb, "@MODULES")
        m["@LIST"] = ",".join(modules)
        self.ldb.add(m)
        self.ldb = samba.Ldb(self.filename)


class TestCaseInTempDir(TestCase):

    def setUp(self):
        super(TestCaseInTempDir, self).setUp()
        self.tempdir = tempfile.mkdtemp()
        self.addCleanup(self._remove_tempdir)

    def _remove_tempdir(self):
        self.assertEquals([], os.listdir(self.tempdir))
        os.rmdir(self.tempdir)
        self.tempdir = None


def env_loadparm():
    lp = param.LoadParm()
    try:
        lp.load(os.environ["SMB_CONF_PATH"])
    except KeyError:
        raise KeyError("SMB_CONF_PATH not set")
    return lp


def env_get_var_value(var_name, allow_missing=False):
    """Returns value for variable in os.environ

    Function throws AssertionError if variable is defined.
    Unit-test based python tests require certain input params
    to be set in environment, otherwise they can't be run
    """
    if allow_missing:
        if var_name not in os.environ.keys():
            return None
    assert var_name in os.environ.keys(), "Please supply %s in environment" % var_name
    return os.environ[var_name]


cmdline_credentials = None

class RpcInterfaceTestCase(TestCase):
    """DCE/RPC Test case."""


class ValidNetbiosNameTests(TestCase):

    def test_valid(self):
        self.assertTrue(samba.valid_netbios_name("FOO"))

    def test_too_long(self):
        self.assertFalse(samba.valid_netbios_name("FOO"*10))

    def test_invalid_characters(self):
        self.assertFalse(samba.valid_netbios_name("*BLA"))


class BlackboxProcessError(Exception):
    """This is raised when check_output() process returns a non-zero exit status

    Exception instance should contain the exact exit code (S.returncode),
    command line (S.cmd), process output (S.stdout) and process error stream
    (S.stderr)
    """

    def __init__(self, returncode, cmd, stdout, stderr):
        self.returncode = returncode
        self.cmd = cmd
        self.stdout = stdout
        self.stderr = stderr

    def __str__(self):
        return "Command '%s'; exit status %d; stdout: '%s'; stderr: '%s'" % (self.cmd, self.returncode,
                                                                             self.stdout, self.stderr)

class BlackboxTestCase(TestCaseInTempDir):
    """Base test case for blackbox tests."""

    def _make_cmdline(self, line):
        bindir = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../../bin"))
        parts = line.split(" ")
        if os.path.exists(os.path.join(bindir, parts[0])):
            parts[0] = os.path.join(bindir, parts[0])
        line = " ".join(parts)
        return line

    def check_run(self, line):
        self.check_exit_code(line, 0)

    def check_exit_code(self, line, expected):
        line = self._make_cmdline(line)
        p = subprocess.Popen(line,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,
                             shell=True)
        stdoutdata, stderrdata = p.communicate()
        retcode = p.returncode
        if retcode != expected:
            raise BlackboxProcessError(retcode,
                                       line,
                                       stdoutdata,
                                       stderrdata)

    def check_output(self, line):
        line = self._make_cmdline(line)
        p = subprocess.Popen(line, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, close_fds=True)
        stdoutdata, stderrdata = p.communicate()
        retcode = p.returncode
        if retcode:
            raise BlackboxProcessError(retcode, line, stdoutdata, stderrdata)
        return stdoutdata


def connect_samdb(samdb_url, lp=None, session_info=None, credentials=None,
                  flags=0, ldb_options=None, ldap_only=False, global_schema=True):
    """Create SamDB instance and connects to samdb_url database.

    :param samdb_url: Url for database to connect to.
    :param lp: Optional loadparm object
    :param session_info: Optional session information
    :param credentials: Optional credentials, defaults to anonymous.
    :param flags: Optional LDB flags
    :param ldap_only: If set, only remote LDAP connection will be created.
    :param global_schema: Whether to use global schema.

    Added value for tests is that we have a shorthand function
    to make proper URL for ldb.connect() while using default
    parameters for connection based on test environment
    """
    if not "://" in samdb_url:
        if not ldap_only and os.path.isfile(samdb_url):
            samdb_url = "tdb://%s" % samdb_url
        else:
            samdb_url = "ldap://%s" % samdb_url
    # use 'paged_search' module when connecting remotely
    if samdb_url.startswith("ldap://"):
        ldb_options = ["modules:paged_searches"]
    elif ldap_only:
        raise AssertionError("Trying to connect to %s while remote "
                             "connection is required" % samdb_url)

    # set defaults for test environment
    if lp is None:
        lp = env_loadparm()
    if session_info is None:
        session_info = samba.auth.system_session(lp)
    if credentials is None:
        credentials = cmdline_credentials

    return SamDB(url=samdb_url,
                 lp=lp,
                 session_info=session_info,
                 credentials=credentials,
                 flags=flags,
                 options=ldb_options,
                 global_schema=global_schema)


def connect_samdb_ex(samdb_url, lp=None, session_info=None, credentials=None,
                     flags=0, ldb_options=None, ldap_only=False):
    """Connects to samdb_url database

    :param samdb_url: Url for database to connect to.
    :param lp: Optional loadparm object
    :param session_info: Optional session information
    :param credentials: Optional credentials, defaults to anonymous.
    :param flags: Optional LDB flags
    :param ldap_only: If set, only remote LDAP connection will be created.
    :return: (sam_db_connection, rootDse_record) tuple
    """
    sam_db = connect_samdb(samdb_url, lp, session_info, credentials,
                           flags, ldb_options, ldap_only)
    # fetch RootDse
    res = sam_db.search(base="", expression="", scope=ldb.SCOPE_BASE,
                        attrs=["*"])
    return (sam_db, res[0])


def connect_samdb_env(env_url, env_username, env_password, lp=None):
    """Connect to SamDB by getting URL and Credentials from environment

    :param env_url: Environment variable name to get lsb url from
    :param env_username: Username environment variable
    :param env_password: Password environment variable
    :return: sam_db_connection
    """
    samdb_url = env_get_var_value(env_url)
    creds = credentials.Credentials()
    if lp is None:
        # guess Credentials parameters here. Otherwise workstation
        # and domain fields are NULL and gencache code segfalts
        lp = param.LoadParm()
        creds.guess(lp)
    creds.set_username(env_get_var_value(env_username))
    creds.set_password(env_get_var_value(env_password))
    return connect_samdb(samdb_url, credentials=creds, lp=lp)


def delete_force(samdb, dn, **kwargs):
    try:
        samdb.delete(dn, **kwargs)
    except ldb.LdbError as error:
        (num, errstr) = error.args
        assert num == ldb.ERR_NO_SUCH_OBJECT, "ldb.delete() failed: %s" % errstr
