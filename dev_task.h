#ifndef DEV_TASK_H
#define DEV_TASK_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define STDERR stderr
#define INPUT_FILE_ARG_INDEX 1

#define NUMERIC_FORMAT "lld"
typedef long long int NumericType;
typedef long double   fNumericType;


int compareNumericTypes(const void* arg1, const void* arg2);
long fileSize(FILE* stream);

#endif