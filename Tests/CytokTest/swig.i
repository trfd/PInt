/* example.i */
%module example
%{
    /* Put header files here or function declarations like below */
    #include "Test.h"
%}

extern void helloWorld(char* c);