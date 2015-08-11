#include <stdio.h>
#include <limits.h>

void main()
{
    char plainChar = 128;
    signed char sChar = 128;
    unsigned char uChar = 128;
    char c;
    short int s;
    int i;
    long int l;

    printf("char      = %d bytes\n", sizeof(c));
    printf("short int = %d bytes\n", sizeof(s));
    printf("int       = %d bytes\n", sizeof(i));
    printf("long int  = %d bytes\n", sizeof(l));
    puts("");

    printf("CHAR_BIT  = %11hhd    CHAR_MIN  = %11hhd    CHAR_MAX  = %11hhd\n", CHAR_BIT, CHAR_MIN, CHAR_MAX);
    printf("SCHAR_MIN = %11hhd    SCHAR_MAX = %11hhd    UCHAR_MAX = %11hhd\n", SCHAR_MIN, SCHAR_MAX, UCHAR_MAX);
    printf("SHRT_MIN  = %11hd    SHRT_MAX  = %11hd    USHRT_MAX = %11hu\n", SHRT_MIN, SHRT_MAX, USHRT_MAX);
    printf("INT_MIN   = %11d    INT_MAX   = %11d    UINT_MAX  = %11u\n", INT_MIN, INT_MAX, UINT_MAX);
    printf("LONG_MIN  = %11ld    LONG_MAX  = %11ld    ULONG_MAX = %11lu\n", LONG_MIN, LONG_MAX, ULONG_MAX);

    /*
    printf("128 in char = %d, 128 in signed char = %d, 128 in unsigned char = %d\n", plainChar, sChar, uChar);
    */
}

