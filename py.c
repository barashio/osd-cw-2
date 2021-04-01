#define PY_SSIZE_T_CLEAN
#include </usr/include/python3.9/Python.h>

int main(int argc, char const *argv[]) {

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);

    if(program == NULL) {
        fprintf(stderr, "Fatal Error: Cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
    Py_Initialize();
    PyRun_SimpleString("print('Hello World!')");

    if(Py_FinalizeEx() < 0) {
        exit(120);
    }

    PyMem_RawFree(program);

    return 0;
}