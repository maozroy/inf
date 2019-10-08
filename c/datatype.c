#include <stdio.h>

int main()
{
char ch = 0;
signed char si_ch = 0;

int in = 0;
unsigned int un_in = 0;

signed short int si_sh_in = 0;
signed long int si_lo_in = 0;

unsigned short int un_sh_in = 0;
unsigned long int un_lo_in = 0;



float fl = 0;
long lo_fl = 0;
double do_fl = 0;
long double lo_do_fl = 0;





puts("\t\tCHARS");
printf("size of (unsigned) char    = %lu bytes\n", sizeof(ch));
printf("size of signed char        = %lu bytes\n", sizeof(si_ch));

puts("\n\t\tINTS");
printf("size of (signed) short int = %lu bytes\n", sizeof(si_sh_in));
printf("size of unsigned short int = %lu bytes\n", sizeof(un_sh_in));
printf("          -----                    \n");
printf("size of (signed) int       = %lu bytes\n", sizeof(in));
printf("size of unsigned int       = %lu bytes\n", sizeof(un_in));
printf("          -----                    \n");
printf("size of (signed) long int  = %lu bytes\n", sizeof(si_lo_in));
printf("size of unsigned long int  = %lu bytes\n", sizeof(un_lo_in));


puts("\n\t\tFLOATS");
printf("size of float              = %lu bytes\n", sizeof(fl));
printf("size of long float         = %lu bytes\n", sizeof(lo_fl));
printf("size of double float       = %lu bytes\n", sizeof(do_fl));
printf("size of long double float  = %lu bytes\n", sizeof(lo_do_fl));


return 0;


}
