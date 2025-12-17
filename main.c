#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>

static PyObject *list_subtract(PyObject *self, PyObject *args)
{
    PyObject *a_obj, *b_obj;
    if (!PyArg_UnpackTuple(args, "list_subtract", 2, 2, &a_obj, &b_obj))
        return NULL;
    PyObject *a_seq = PySequence_Fast(a_obj, "arg1 must be a sequence");
    if (!a_seq) return NULL;
    PyObject *b_seq = PySequence_Fast(b_obj, "arg2 must be a sequence");
    if (!b_seq) { Py_DECREF(a_seq); return NULL; }
    Py_ssize_t n = PySequence_Fast_GET_SIZE(a_seq);
    if (n != PySequence_Fast_GET_SIZE(b_seq)) {
        Py_DECREF(a_seq);
        Py_DECREF(b_seq);
        PyErr_SetString(PyExc_ValueError, "sequences must have the same length");
        return NULL;
    }
    PyObject *res = PyList_New(n);
    if (!res) { Py_DECREF(a_seq); Py_DECREF(b_seq); return NULL; }
    for (Py_ssize_t i = 0; i < n; ++i) {
        PyObject *ai = PySequence_Fast_GET_ITEM(a_seq, i);
        PyObject *bi = PySequence_Fast_GET_ITEM(b_seq, i);
        PyObject *d = PyNumber_Subtract(ai, bi);
        if (!d) {
            Py_DECREF(res);
            Py_DECREF(a_seq);
            Py_DECREF(b_seq);
            return NULL;
        }
        PyList_SET_ITEM(res, i, d);
    }
    Py_DECREF(a_seq);
    Py_DECREF(b_seq);
    return res;
}

static PyObject *benchmark_subtract(PyObject *self, PyObject *args)
{
    PyObject *a_obj, *b_obj;
    unsigned long iterations = 1;
    if (!PyArg_ParseTuple(args, "OO|k", &a_obj, &b_obj, &iterations))
        return NULL;
    clock_t start = clock();
    for (unsigned long it = 0; it < iterations; ++it) {
        PyObject *call_args = PyTuple_Pack(2, a_obj, b_obj);
        if (!call_args) return NULL;
        PyObject *tmp = list_subtract(NULL, call_args);
        Py_DECREF(call_args);
        if (!tmp) return NULL;
        Py_DECREF(tmp);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / (double)CLOCKS_PER_SEC;
    return PyFloat_FromDouble(elapsed);
}

static PyMethodDef Methods[] = {
    {"list_subtract", (PyCFunction)list_subtract, METH_VARARGS, NULL},
    {"benchmark_subtract", (PyCFunction)benchmark_subtract, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mylib",
    NULL,
    -1,
    Methods
};

PyMODINIT_FUNC
PyInit_mylib(void)
{
    return PyModule_Create(&moduledef);
}