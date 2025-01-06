#include "memory.c"
#include <stdio.h>

/*

Example code to demonstrate the use of mtk_memcpy

*/
int main(){
    char buffer[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    char dest[10];

    mtk_memcpy(dest, buffer, 10);

    dest[9] = '\0';

    // Prints buffer
    printf("%s\n", dest);

    for (int i=0; i<32; i++) {
        if(dest[i] == '\0'){
            printf("--NULL TERMINATOR HERE--");
        }
        printf("%c", dest[i]);
    }
    printf("\n");
}
