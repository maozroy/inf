#ifndef DB_MY_UTILS
#define DB_MY_UTILS

#include <stdio.h>      /* For fprintf  */

#define RED "\x1B[01;31m"
#define GRN "\x1B[01;32m"
#define YEL "\x1B[01;33m"
#define RESET "\x1B[0m"

#define TRUE 1
#define FALSE 0

#define FAILURE 1
#define SUCCESS 0

#define DEADBEEF (void *)0xDEADBEEF

#define VLGCHK printf(YEL"Check valgrind for any memory leaks\n\n"RESET)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define USE_UNUSED(var) (void)(var)

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n",\
(name), (num), (test ? GRN"Passed"RESET : RED"Failed"RESET))

#endif