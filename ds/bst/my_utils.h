#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define NUM_FOR_TESTING 5
#define MAX(a, b)((a > b) ? (a): (b))
#define MIN(a, b)((a < b) ? (a): (b))
#define MAX3(a, b, c)(MAX(a, b) > c ? MAX(a,b) : c)
#define UNUSED(x) (void)(x)

#define RUN_TEST(TEST,NAME) \
	(TEST) ? \
	printf("Test"KBLU" %s"KGRN" PASS\n"KNRM, (NAME)) : \
	printf("Test"KBLU" %s"KRED" FAIL\n"KNRM, (NAME));

 
 
