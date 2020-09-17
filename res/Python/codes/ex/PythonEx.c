#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>

int fac(int n)
{
	if (n < 2) {
		return (1);
	}
	else {
		return (n) * fac(n - 1);
	}
}

char* reverse(char *s)
{
	register char t;
	register char *p = s;
	register char *q = (s + strlen(s) -1);

	while (p < q) {
		t = *p;
		*p++ = *q;
		*q-- = t;
	}

	return s;
}

int test()
{
	char s[BUFSIZ];

	printf("3! = %d\n", fac(3));
	printf("6! = %d\n", fac(6));
	printf("9! = %d\n", fac(9));

	strcpy(s, "abcdefg");
	printf("'abcdefg' after reversing is '%s'\n", reverse(s));
	strcpy(s, "python");
	printf("'python' after reversing is '%s'\n", reverse(s));

	return 0;
}

static PyObject* CustomPy_fac(PyObject *self, PyObject *args)
{
	int num;
	if (!PyArg_ParseTuple(args, "i", &num)) { // i i.e. int->int
		return NULL;
	}
	return (PyObject*)Py_BuildValue("i", fac(num)); // i i.e. int->int
}

static PyObject* CustomPy_reverse(PyObject *self, PyObject *args)
{
	char *orig_str; // original
	char *dup_str; // reversed
	PyObject *retval;
	if (!PyArg_ParseTuple(args, "s", &orig_str)) { // s i.e. str->char*
		return NULL;
	}
	retval = (PyObject*)Py_BuildValue(
			"ss", // s i.e. char*->str
			orig_str,
			dup_str = reverse(strdup(orig_str))); // strdup
	free(dup_str); // free after strdup
	return retval; // return tuple(orig_str, dup_str)
}

static PyObject* CustomPy_test(PyObject *self, PyObject *args)
{
	test();
	return (PyObject*)Py_BuildValue("");
}

static PyMethodDef CustomPyMethods[] = {
	{"fac", CustomPy_fac, METH_VARARGS},
	{"reverse", CustomPy_reverse, METH_VARARGS},
	{"test", CustomPy_test, METH_VARARGS},
	{NULL, NULL},
};

void initCustomPy()
{
	Py_InitModule("CustomPy", CustomPyMethods);
}