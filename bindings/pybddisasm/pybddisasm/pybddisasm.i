/*
 * Copyright (c) 2023 Bitdefender
 * SPDX-License-Identifier: Apache-2.0
 */
%module (package="pybddisasm", moduleimport="import $module") pybddisasm
%{
    #define SWIG_FILE_WITH_INIT

    #include "pybddisasm.h"

    #include "bddisasm.h"
    #include "constants.h"
    #include "cpuidflags.h"
    #include "disasmstatus.h"
    #include "disasmtypes.h"
    #include "registers.h"
    #include "version.h"

%}

%include "typemaps.i"
%include "stdint.i"

%{
    #define ND_ARCH_X64
    static PyObject * __uint8_to_pylist(uint8_t *arr, size_t size)
    {
        PyObject *list = PyList_New(size);
        for (uint8_t idx = 0; idx < size; idx++) {
            PyObject *obj = PyInt_FromLong(arr[idx]);
            PyList_SetItem(list, idx, obj);
        }

        return list;
    }

    static PyObject * __nd_operand_to_pylist(ND_OPERAND *arr, size_t size)
    {
        PyObject *list = PyList_New(size);

        for (uint8_t idx = 0; idx < size; idx++) {
            PyObject *obj = SWIG_NewPointerObj(&arr[idx], SWIGTYPE_p__ND_OPERAND, 0);
            PyList_SetItem(list, idx, obj);
        }

        return list;
    }
%}

%typemap(in,numinputs=0) ND_UINT32 * (ND_UINT32 tmp) %{
    $1 = &tmp;
%}

%typemap(argout) ND_UINT32 * (PyObject* obj) %{
    obj = PyLong_FromLong(*$1);
    $result = SWIG_Python_AppendOutput($result,obj);
%}

%typemap(freearg) ND_UINT32 * %{

%}

%typemap(in,numinputs=0) char** (char* tmp) %{
    $1 = &tmp;
%}

%typemap(argout) char** (PyObject* obj) %{
    obj = PyUnicode_FromString(*$1);
    $result = SWIG_Python_AppendOutput($result,obj);
%}

%typemap(freearg) char** %{

%}


%typemap(in, numinputs = 0) (ND_UINT32 buffer_size, char *__output_buffer) {
   $1 = ND_MIN_BUF_SIZE;
   $2 = (char *)malloc($1 * sizeof(char));
}

%typemap(freearg) (ND_UINT32 buffer_size, char *__output_buffer) {
   free($2);
}

%typemap(argout) char *__output_buffer {
    // TODO: check if function failed
    PyObject *o, *o2, *o3;
    o = PyString_FromString($1);
    if ((!$result) || ($result == Py_None)) {
        $result = o;
    } else {
        if (!PyTuple_Check($result)) {
            PyObject *o2 = $result;
            $result = PyTuple_New(1);
            PyTuple_SetItem($result,0,o2);
        }
        o3 = PyTuple_New(1);
        PyTuple_SetItem(o3,0,o);
        o2 = $result;
        $result = PySequence_Concat(o2,o3);
        Py_DECREF(o2);
        Py_DECREF(o3);
    }
}

%typemap(in, numinputs=0) INSTRUX * __output_instr {
    $1 = malloc(sizeof(INSTRUX));

}

%typemap(freearg) INSTRUX * __output_instr {
}

%typemap(argout) INSTRUX * __output_instr  {
    %append_output(SWIG_NewPointerObj($1,$1_descriptor,SWIG_POINTER_OWN));
}

%typemap(in,numinputs=0,noblock=1) size_t *__output_convert_length {
  size_t tmp_len;
  $1 = &tmp_len;
}

%typemap(out) ND_UINT8 * {
  size_t size = tmp_len;
  $result = __uint8_to_pylist($1, size);
}

%typemap(out) ND_OPERAND * {
  size_t size = tmp_len;
  $result = __nd_operand_to_pylist($1, size);
}

%typemap(in, numinputs = 1) (const ND_UINT8 *) {
    Py_buffer view;

    if (!(PyObject_CheckBuffer($input) || PyBytes_Check($input) || PyByteArray_Check($input))) {
        SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument "
                "$argnum"" of type '" "$type""'");
    }

    if (PyObject_GetBuffer($input, &view, 0) != 0) {
        SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument "
            "$argnum"" of type '" "$type""'");
    }

    $1 = (uint8_t *)malloc(sizeof(uint8_t) * view.len);
    if (!($1)) {
        SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument "
            "$argnum"" of type '" "$type""'");
    }

    if (PyBuffer_ToContiguous($1, &view, view.len, 'C') != 0) {
        SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument "
            "$argnum"" of type '" "$type""'");
    }

    PyBuffer_Release(&view);
}

%typemap(freearg) (const ND_UINT8 *) {
    free($1);
}

%define __x86_64__
%enddef


%include "bddisasm.h"
%include "constants.h"
%include "cpuidflags.h"
%include "disasmstatus.h"
%include "disasmtypes.h"
%include "registers.h"
%include "version.h"

%include "pybddisasm.h"
