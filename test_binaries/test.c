#include <stdio.h>
#include "test.h"
__attribute__((weak)) int my_weak_variable = 10;
extern __attribute__((weak)) int otherweak;
extern void *yolo();
void test();
int bss_loc;
static int bss_glob;
int main() {
    printf("%d\n", my_weak_variable);
    return 0;
}