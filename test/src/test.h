
#pragma once
#include <stdio.h>
#include <stdlib.h>

#define TEST(form, format, ...)\
if (form){\
printf("[ok] %s:%d %s: " format "\n", __FILE__, __LINE__, #form __VA_OPT__(,) __VA_ARGS__);\
}\
else {\
printf("[fail] %s:%d %s: " format "\n", __FILE__, __LINE__, #form __VA_OPT__(,) __VA_ARGS__);\
abort();\
}
