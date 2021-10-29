/*
   Unix SMB/CIFS implementation.
   Samba crypto functions

   Copyright (C) Alexander Bokovoy <ab@samba.org> 2017

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Python.h>
#include "includes.h"
#include "python/py3compat.h"
#include "lib/crypto/arcfour.h"

static PyObject *py_crypto_arcfour_crypt_blob(PyObject *module, PyObject *args, PyObject *kwargs)
{
	DATA_BLOB data, key;
	PyObject *py_data, *py_key, *result;
	TALLOC_CTX *ctx;

	if (!PyArg_ParseTuple(args, "OO", &py_data, &py_key))
		return NULL;

	if (!PyBytes_Check(py_data)) {
		PyErr_Format(PyExc_TypeError, "bytes expected");
		return NULL;
	}

	if (!PyBytes_Check(py_key)) {
		PyErr_Format(PyExc_TypeError, "bytes expected");
		return NULL;
	}

	ctx = talloc_new(NULL);

	data.length = PyBytes_Size(py_data);
	data.data = talloc_memdup(ctx, PyBytes_AsString(py_data), data.length);
	if (!data.data) {
		talloc_free(ctx);
		return PyErr_NoMemory();
	}

	key.data = (uint8_t *)PyBytes_AsString(py_key);
	key.length = PyBytes_Size(py_key);

	arcfour_crypt_blob(data.data, data.length, &key);

	result = PyBytes_FromStringAndSize((const char*) data.data, data.length);
	talloc_free(ctx);
	return result;
}


static const char py_crypto_arcfour_crypt_blob_doc[] = "arcfour_crypt_blob(data, key)\n"
					 "Encrypt the data with RC4 algorithm using the key";

static PyMethodDef py_crypto_methods[] = {
	{ "arcfour_crypt_blob", (PyCFunction)py_crypto_arcfour_crypt_blob, METH_VARARGS, py_crypto_arcfour_crypt_blob_doc },
	{ NULL },
};

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	.m_name = "crypto",
	.m_doc = "Crypto functions required for SMB",
	.m_size = -1,
	.m_methods = py_crypto_methods,
};

MODULE_INIT_FUNC(crypto)
{
	PyObject *m;

	m = PyModule_Create(&moduledef);
	if (m == NULL)
		return NULL;

	return m;
}
