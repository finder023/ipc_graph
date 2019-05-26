#include <stdio.h>
#include <stdarg.h>

#define dprintf(f_, ...) printf((f_), ##__VA_ARGS__)
#define my_print(...) 
int main() {
    printf("\n");
    char *name = "proc1"

    my_print("%s_2_%s\n", "proc1", name);
    my_print("hello\n");
    
    
    dprintf("%s_2_%s\n", "proc1", name);
    dprintf("hello\n");
}