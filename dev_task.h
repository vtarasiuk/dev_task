#ifndef DEV_TASK_H
#define DEV_TASK_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <search.h>
#include <malloc.h>

#define STDERR stderr
#define INPUT_FILE_ARG_INDEX 1

#define NUMERIC_FORMAT "lld"
typedef long long int NumericType;
typedef long double   fNumericType;


inline NumericType calculateMaxNumber(NumericType max, NumericType value);
int compareNumericTypes(const void* arg1, const void* arg2);
long fileSize(FILE* stream);

#endif