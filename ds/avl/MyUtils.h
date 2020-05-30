#ifndef ILRD_MYUTILS_H
#define ILRD_MYUTILS_H

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define MIN2(a,b) (a) <= (b) ? (a) : (b)
#define MAX2(a,b) (a) >= (b) ? (a) : (b)
#define MIN3(a,b,c) MIN2(a,b) <= MIN2(b,c) ? (MIN2(a,b)) : MIN2(b,c)
#define MAX3(a,b,c) MAX2(a,b) >= MAX2(b,c) ? (MAX2(a,b)) : MAX2(b,c)

#define PRINTTESTRESULTS(func,num, res) \
(printf("%-55s: Test %d %s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))

#endif
