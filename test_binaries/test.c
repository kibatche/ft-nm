#include <stdio.h>
#include "test.h"
typedef int TEST;
char __attribute__((section(".scommon"))) SMALL_COMMON;
char __attribute__((section(".scommon"))) SMALL_COMMON;
__attribute__((weak)) int WEAK_VAR_CANARY = 10; // V
const int __attribute__((weak)) RODATA_GLOB_CANARY;// R .rodata
int TEXT_CANARY(int,int);// T
int COMMON_VAR_GLOB_CANARY; // "C" quand on fait gcc -fcommon -c a.c test.c  et qu'on test le .o résultat => nm test.o sinon ça va dans .bss
extern int __attribute__((weak)) COMON_VAR_LOC_CANARY; // gcc -fcommon -c a.c test.c 
int BSS_GLOB_CANARY; // B
static int BSS_LOC_CANARY; // b
int DATA_GLOB_CANARY = 42; // data GLOBAL (D)
static int DATA_LOC_CANARY = 42; // data (d)
int TEXT_CANARY(int x, int y)// T
{
    return x*y;
}

int main() {
    printf("%d\n", 10);
    struct TESTSTRUCT *i = malloc(sizeof(S_TESTSTRUCT));
    S_TESTSTRUCT *j = NULL;
    int a = X(1);
    return 0;
}