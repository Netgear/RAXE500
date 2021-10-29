#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This tests the password lockout behavior for AD implementations
#
# Copyright Matthias Dieter Wallnoefer 2010
# Copyright Andrew Bartlett 2013
# Copyright Stefan Metzmacher 2014
#

import optparse
import sys
import base64
import time

sys.path.insert(0, "bin/python")
import samba

from samba.tests.subunitrun import TestProgram, SubunitOptions

import samba.getopt as options

from samba.auth import system_session
from samba.credentials import Credentials, DONT_USE_KERBEROS, MUST_USE_KERBEROS
from ldb import SCOPE_BASE, LdbError
from ldb import ERR_CONSTRAINT_VIOLATION
from ldb import ERR_INVALID_CREDENTIALS
from ldb import Message, MessageElement, Dn
from ldb import FLAG_MOD_REPLACE
from samba import gensec, dsdb
from samba.samdb import SamDB
import samba.tests
from samba.tests import delete_force
from samba.dcerpc import security, samr
from samba.ndr import ndr_unpack

parser = optparse.OptionParser("password_lockout.py [options] <host>")
sambaopts = options.SambaOptions(parser)
parser.add_option_group(sambaopts)
parser.add_option_group(options.VersionOptions(parser))
# use command line creds if available
credopts = options.CredentialsOptions(parser)
parser.add_option_group(credopts)
subunitopts = SubunitOptions(parser)
parser.add_option_group(subunitopts)
opts, args = parser.parse_args()

if len(args) < 1:
    parser.print_usage()
    sys.exit(1)

host = args[0]

lp = sambaopts.get_loadparm()
global_creds = credopts.get_credentials(lp)

import password_lockout_base

#
# Tests start here
#

class PasswordTests(password_lockout_base.BasePasswordTestCase):
    def setUp(self):
        self.host = host
        self.host_url = host_url
        self.lp = lp
        self.global_creds = global_creds
        self.ldb = SamDB(url=self.host_url, session_info=system_session(self.lp),
                         credentials=self.global_creds, lp=self.lp)
        super(PasswordTests, self).setUp()

        self.lockout2krb5_creds = self.insta_creds(self.template_creds,
                                                   username="lockout2krb5",
                                                   userpass="thatsAcomplPASS0",
                                                   kerberos_state=MUST_USE_KERBEROS)
        self.lockout2krb5_ldb = self._readd_user(self.lockout2krb5_creds,
                                                 lockOutObservationWindow=self.lockout_observation_window)

        self.lockout2ntlm_creds = self.insta_creds(self.template_creds,
                                                   username="lockout2ntlm",
                                                   userpass="thatsAcomplPASS0",
                                                   kerberos_state=DONT_USE_KERBEROS)
        self.lockout2ntlm_ldb = self._readd_user(self.lockout2ntlm_creds,
                                                 lockOutObservationWindow=self.lockout_observation_window)

    def _reset_ldap_lockoutTime(self, res):
        self.ldb.modify_ldif("""
dn: """ + str(res[0].dn) + """
changetype: modify
replace: lockoutTime
lockoutTime: 0
""")

    def _reset_ldap_userAccountControl(self, res):
        self.assertTrue("userAccountControl" in res[0])
        self.assertTrue("msDS-User-Account-Control-Computed" in res[0])

        uac = int(res[0]["userAccountControl"][0])
        uacc = int(res[0]["msDS-User-Account-Control-Computed"][0])

        uac |= uacc
        uac = uac & ~dsdb.UF_LOCKOUT

        self.ldb.modify_ldif("""
dn: """ + str(res[0].dn) + """
changetype: modify
replace: userAccountControl
userAccountControl: %d
""" % uac)

    def _reset_by_method(self, res, method):
        if method is "ldap_userAccountControl":
            self._reset_ldap_userAccountControl(res)
        elif method is "ldap_lockoutTime":
            self._reset_ldap_lockoutTime(res)
        elif method is "samr":
            self._reset_samr(res)
        else:
            self.assertTrue(False, msg="Invalid reset method[%s]" % method)

    def _test_userPassword_lockout_with_clear_change(self, creds, other_ldb, method,
                                                     initial_lastlogon_relation=None):
        # Notice: This works only against Windows if "dSHeuristics" has been set
        # properly
        username = creds.get_username()
        userpass = creds.get_password()
        userdn = "cn=%s,cn=users,%s" % (username, self.base_dn)

        use_kerberos = creds.get_kerberos_state()
        if use_kerberos == MUST_USE_KERBEROS:
            logoncount_relation = 'greater'
            lastlogon_relation = 'greater'
            print "Performs a password cleartext change operation on 'userPassword' using Kerberos"
        else:
            logoncount_relation = 'equal'
            lastlogon_relation = 'equal'
            print "Performs a password cleartext change operation on 'userPassword' using NTLMSSP"

        if initial_lastlogon_relation is not None:
            lastlogon_relation = initial_lastlogon_relation

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=("greater", 0),
                                  logonCount=(logoncount_relation, 0),
                                  lastLogon=(lastlogon_relation, 0),
                                  lastLogonTimestamp=('greater', 0),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])
        logonCount = int(res[0]["logonCount"][0])
        lastLogon = int(res[0]["lastLogon"][0])
        lastLogonTimestamp = int(res[0]["lastLogonTimestamp"][0])
        if lastlogon_relation == 'greater':
            self.assertGreater(lastLogon, badPasswordTime)
            self.assertGreaterEqual(lastLogon, lastLogonTimestamp)

        # Change password on a connection as another user

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1x
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # Correct old password
        other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: """ + userpass + """
add: userPassword
userPassword: thatsAcomplPASS2
""")

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1x
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        print "two failed password change"

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1x
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=("greater", badPasswordTime),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)
        badPasswordTime = int(res[0]["badPasswordTime"][0])
        lockoutTime = int(res[0]["lockoutTime"][0])

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1x
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1x
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lockoutTime=lockoutTime,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        try:
            # Correct old password
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS2
add: userPassword
userPassword: thatsAcomplPASS2x
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        # Now reset the password, which does NOT change the lockout!
        self.ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
replace: userPassword
userPassword: thatsAcomplPASS2
""")

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        try:
            # Correct old password
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS2
add: userPassword
userPassword: thatsAcomplPASS2x
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        m = Message()
        m.dn = Dn(self.ldb, userdn)
        m["userAccountControl"] = MessageElement(
          str(dsdb.UF_LOCKOUT),
          FLAG_MOD_REPLACE, "userAccountControl")

        self.ldb.modify(m)

        # This shows that setting the UF_LOCKOUT flag alone makes no difference
        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        # This shows that setting the UF_LOCKOUT flag makes no difference
        try:
            # Correct old password
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS2\"".encode('utf-16-le')) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS2x\"".encode('utf-16-le')) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lockoutTime=lockoutTime,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        self._reset_by_method(res, method)

        # Here bad password counts are reset without logon success.
        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lockoutTime=0,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # The correct password after doing the unlock

        other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS2\"".encode('utf-16-le')) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS2x\"".encode('utf-16-le')) + """
""")
        userpass = "thatsAcomplPASS2x"
        creds.set_password(userpass)

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lockoutTime=0,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1xyz
add: userPassword
userPassword: thatsAcomplPASS2XYZ
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lockoutTime=0,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: thatsAcomplPASS1xyz
add: userPassword
userPassword: thatsAcomplPASS2XYZ
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lockoutTime=0,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        self._reset_ldap_lockoutTime(res)

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

    def test_userPassword_lockout_with_clear_change_krb5_ldap_userAccountControl(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1krb5_creds,
                                                          self.lockout2krb5_ldb,
                                                          "ldap_userAccountControl")

    def test_userPassword_lockout_with_clear_change_krb5_ldap_lockoutTime(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1krb5_creds,
                                                          self.lockout2krb5_ldb,
                                                          "ldap_lockoutTime")

    def test_userPassword_lockout_with_clear_change_krb5_samr(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1krb5_creds,
                                                          self.lockout2krb5_ldb,
                                                          "samr")

    def test_userPassword_lockout_with_clear_change_ntlm_ldap_userAccountControl(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1ntlm_creds,
                                                          self.lockout2ntlm_ldb,
                                                          "ldap_userAccountControl",
                                                          initial_lastlogon_relation='greater')

    def test_userPassword_lockout_with_clear_change_ntlm_ldap_lockoutTime(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1ntlm_creds,
                                                          self.lockout2ntlm_ldb,
                                                          "ldap_lockoutTime",
                                                          initial_lastlogon_relation='greater')

    def test_userPassword_lockout_with_clear_change_ntlm_samr(self):
        self._test_userPassword_lockout_with_clear_change(self.lockout1ntlm_creds,
                                                          self.lockout2ntlm_ldb,
                                                          "samr",
                                                          initial_lastlogon_relation='greater')

    def _test_unicodePwd_lockout_with_clear_change(self, creds, other_ldb,
                                                   initial_logoncount_relation=None):
        print "Performs a password cleartext change operation on 'unicodePwd'"
        username = creds.get_username()
        userpass = creds.get_password()
        userdn = "cn=%s,cn=users,%s" % (username, self.base_dn)
        if initial_logoncount_relation is not None:
            logoncount_relation = initial_logoncount_relation
        else:
            logoncount_relation = "greater"

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=("greater", 0),
                                  logonCount=(logoncount_relation, 0),
                                  lastLogon=("greater", 0),
                                  lastLogonTimestamp=("greater", 0),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])
        logonCount = int(res[0]["logonCount"][0])
        lastLogon = int(res[0]["lastLogon"][0])
        lastLogonTimestamp = int(res[0]["lastLogonTimestamp"][0])
        self.assertGreater(lastLogonTimestamp, badPasswordTime)
        self.assertGreaterEqual(lastLogon, lastLogonTimestamp)

        # Change password on a connection as another user

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS1x\"".encode('utf-16-le')) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode("\"thatsAcomplPASS2\"".encode('utf-16-le')) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # Correct old password
        old_utf16 = ("\"%s\"" % userpass).encode('utf-16-le')
        invalid_utf16 = "\"thatsAcomplPASSX\"".encode('utf-16-le')
        userpass = "thatsAcomplPASS2"
        creds.set_password(userpass)
        new_utf16 = ("\"%s\"" % userpass).encode('utf-16-le')

        other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(old_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(old_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # SAMR doesn't have any impact if dsdb.UF_LOCKOUT isn't present.
        # It doesn't create "lockoutTime" = 0 and doesn't
        # reset "badPwdCount" = 0.
        self._reset_samr(res)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        print "two failed password change"

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        # this is strange, why do we have lockoutTime=badPasswordTime here?
        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=("greater", badPasswordTime),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)
        badPasswordTime = int(res[0]["badPasswordTime"][0])
        lockoutTime = int(res[0]["lockoutTime"][0])

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        try:
            # Correct old password
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000775' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)

        # Now reset the lockout, by removing ACB_AUTOLOCK (which removes the lock, despite being a generated attribute)
        self._reset_samr(res);

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Correct old password
        old_utf16 = ("\"%s\"" % userpass).encode('utf-16-le')
        invalid_utf16 = "\"thatsAcomplPASSiX\"".encode('utf-16-le')
        userpass = "thatsAcomplPASS2x"
        creds.set_password(userpass)
        new_utf16 = ("\"%s\"" % userpass).encode('utf-16-le')

        other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(old_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # SAMR doesn't have any impact if dsdb.UF_LOCKOUT isn't present.
        # It doesn't reset "badPwdCount" = 0.
        self._reset_samr(res)

        res = self._check_account(userdn,
                                  badPwdCount=2,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=0,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # Wrong old password
        try:
            other_ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: unicodePwd
unicodePwd:: """ + base64.b64encode(invalid_utf16) + """
add: unicodePwd
unicodePwd:: """ + base64.b64encode(new_utf16) + """
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=3,
                                  badPasswordTime=("greater", badPasswordTime),
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=("greater", badPasswordTime),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=dsdb.UF_LOCKOUT)
        badPasswordTime = int(res[0]["badPasswordTime"][0])
        lockoutTime = int(res[0]["lockoutTime"][0])

        time.sleep(self.account_lockout_duration + 1)

        res = self._check_account(userdn,
                                  badPwdCount=3, effective_bad_password_count=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  lockoutTime=lockoutTime,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        # SAMR doesn't have any impact if dsdb.UF_LOCKOUT isn't present.
        # It doesn't reset "lockoutTime" = 0 and doesn't
        # reset "badPwdCount" = 0.
        self._reset_samr(res)

        res = self._check_account(userdn,
                                  badPwdCount=3, effective_bad_password_count=0,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lockoutTime=lockoutTime,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

    def test_unicodePwd_lockout_with_clear_change_krb5(self):
        self._test_unicodePwd_lockout_with_clear_change(self.lockout1krb5_creds,
                                                        self.lockout2krb5_ldb)

    def test_unicodePwd_lockout_with_clear_change_ntlm(self):
        self._test_unicodePwd_lockout_with_clear_change(self.lockout1ntlm_creds,
                                                        self.lockout2ntlm_ldb,
                                                        initial_logoncount_relation="equal")

    def test_login_lockout_krb5(self):
        self._test_login_lockout(self.lockout1krb5_creds)

    def test_login_lockout_ntlm(self):
        self._test_login_lockout(self.lockout1ntlm_creds)

    def test_multiple_logon_krb5(self):
        self._test_multiple_logon(self.lockout1krb5_creds)

    def test_multiple_logon_ntlm(self):
        self._test_multiple_logon(self.lockout1ntlm_creds)

    def _testing_add_user(self, creds, lockOutObservationWindow=0):
        username = creds.get_username()
        userpass = creds.get_password()
        userdn = "cn=%s,cn=users,%s" % (username, self.base_dn)

        use_kerberos = creds.get_kerberos_state()
        if use_kerberos == MUST_USE_KERBEROS:
            logoncount_relation = 'greater'
            lastlogon_relation = 'greater'
        else:
            logoncount_relation = 'equal'
            if lockOutObservationWindow == 0:
                lastlogon_relation = 'greater'
            else:
                lastlogon_relation = 'equal'

        delete_force(self.ldb, userdn)
        self.ldb.add({
             "dn": userdn,
             "objectclass": "user",
             "sAMAccountName": username})

        self.addCleanup(delete_force, self.ldb, userdn)

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=0,
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT |
                                    dsdb.UF_ACCOUNTDISABLE |
                                    dsdb.UF_PASSWD_NOTREQD,
                                  msDSUserAccountControlComputed=
                                    dsdb.UF_PASSWORD_EXPIRED)

        # SAMR doesn't have any impact if dsdb.UF_LOCKOUT isn't present.
        # It doesn't create "lockoutTime" = 0.
        self._reset_samr(res)

        res = self._check_account(userdn,
                                  badPwdCount=0,
                                  badPasswordTime=0,
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT |
                                    dsdb.UF_ACCOUNTDISABLE |
                                    dsdb.UF_PASSWD_NOTREQD,
                                  msDSUserAccountControlComputed=
                                    dsdb.UF_PASSWORD_EXPIRED)

        # Tests a password change when we don't have any password yet with a
        # wrong old password
        try:
            self.ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
userPassword: noPassword
add: userPassword
userPassword: thatsAcomplPASS2
""")
            self.fail()
        except LdbError, (num, msg):
            self.assertEquals(num, ERR_CONSTRAINT_VIOLATION)
            # Windows (2008 at least) seems to have some small bug here: it
            # returns "0000056A" on longer (always wrong) previous passwords.
            self.assertTrue('00000056' in msg, msg)

        res = self._check_account(userdn,
                                  badPwdCount=1,
                                  badPasswordTime=("greater", 0),
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT |
                                    dsdb.UF_ACCOUNTDISABLE |
                                    dsdb.UF_PASSWD_NOTREQD,
                                  msDSUserAccountControlComputed=
                                    dsdb.UF_PASSWORD_EXPIRED)
        badPwdCount = int(res[0]["badPwdCount"][0])
        badPasswordTime = int(res[0]["badPasswordTime"][0])

        # Sets the initial user password with a "special" password change
        # I think that this internally is a password set operation and it can
        # only be performed by someone which has password set privileges on the
        # account (at least in s4 we do handle it like that).
        self.ldb.modify_ldif("""
dn: """ + userdn + """
changetype: modify
delete: userPassword
add: userPassword
userPassword: """ + userpass + """
""")

        res = self._check_account(userdn,
                                  badPwdCount=badPwdCount,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT |
                                    dsdb.UF_ACCOUNTDISABLE |
                                    dsdb.UF_PASSWD_NOTREQD,
                                  msDSUserAccountControlComputed=0)

        # Enables the user account
        self.ldb.enable_account("(sAMAccountName=%s)" % username)

        res = self._check_account(userdn,
                                  badPwdCount=badPwdCount,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        if lockOutObservationWindow != 0:
            time.sleep(lockOutObservationWindow + 1)
            effective_bad_password_count = 0
        else:
            effective_bad_password_count = badPwdCount

        res = self._check_account(userdn,
                                  badPwdCount=badPwdCount,
                                  effective_bad_password_count=effective_bad_password_count,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=0,
                                  lastLogon=0,
                                  lastLogonTimestamp=('absent', None),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        ldb = SamDB(url=self.host_url, credentials=creds, lp=self.lp)

        if lockOutObservationWindow == 0:
            badPwdCount = 0
            effective_bad_password_count = 0
        if use_kerberos == MUST_USE_KERBEROS:
            badPwdCount = 0
            effective_bad_password_count = 0

        res = self._check_account(userdn,
                                  badPwdCount=badPwdCount,
                                  effective_bad_password_count=effective_bad_password_count,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=(logoncount_relation, 0),
                                  lastLogon=(lastlogon_relation, 0),
                                  lastLogonTimestamp=('greater', badPasswordTime),
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)

        logonCount = int(res[0]["logonCount"][0])
        lastLogon = int(res[0]["lastLogon"][0])
        lastLogonTimestamp = int(res[0]["lastLogonTimestamp"][0])
        if lastlogon_relation == 'greater':
            self.assertGreater(lastLogon, badPasswordTime)
            self.assertGreaterEqual(lastLogon, lastLogonTimestamp)

        res = self._check_account(userdn,
                                  badPwdCount=badPwdCount,
                                  effective_bad_password_count=effective_bad_password_count,
                                  badPasswordTime=badPasswordTime,
                                  logonCount=logonCount,
                                  lastLogon=lastLogon,
                                  lastLogonTimestamp=lastLogonTimestamp,
                                  userAccountControl=
                                    dsdb.UF_NORMAL_ACCOUNT,
                                  msDSUserAccountControlComputed=0)
        return ldb

    def _reset_samr(self, res):

        # Now reset the lockout, by removing ACB_AUTOLOCK (which removes the lock, despite being a generated attribute)
        samr_user = self._open_samr_user(res)
        acb_info = self.samr.QueryUserInfo(samr_user, 16)
        acb_info.acct_flags &= ~samr.ACB_AUTOLOCK
        self.samr.SetUserInfo(samr_user, 16, acb_info)
        self.samr.Close(samr_user)

    def test_lockout_observation_window(self):
        lockout3krb5_creds = self.insta_creds(self.template_creds,
                                              username="lockout3krb5",
                                              userpass="thatsAcomplPASS0",
                                              kerberos_state=MUST_USE_KERBEROS)
        self._testing_add_user(lockout3krb5_creds)

        lockout4krb5_creds = self.insta_creds(self.template_creds,
                                              username="lockout4krb5",
                                              userpass="thatsAcomplPASS0",
                                              kerberos_state=MUST_USE_KERBEROS)
        self._testing_add_user(lockout4krb5_creds,
                               lockOutObservationWindow=self.lockout_observation_window)

        lockout3ntlm_creds = self.insta_creds(self.template_creds,
                                              username="lockout3ntlm",
                                              userpass="thatsAcomplPASS0",
                                              kerberos_state=DONT_USE_KERBEROS)
        self._testing_add_user(lockout3ntlm_creds)
        lockout4ntlm_creds = self.insta_creds(self.template_creds,
                                              username="lockout4ntlm",
                                              userpass="thatsAcomplPASS0",
                                              kerberos_state=DONT_USE_KERBEROS)
        self._testing_add_user(lockout4ntlm_creds,
                               lockOutObservationWindow=self.lockout_observation_window)

host_url = "ldap://%s" % host

TestProgram(module=__name__, opts=subunitopts)
