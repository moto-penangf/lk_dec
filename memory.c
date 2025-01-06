#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/*

This is a simplified version of the memcpy function from LK.

Memory alignment is not included, but it doesn't affect the output of the function

Disassembled with the help of AI

Some interesting notes:
- The code checks whether the memory overlaps and does the copy in the correct direction accordingly
- No checks for the destination size are done, meaning it is prone to buffer oveflows

*/
void *mtk_memcpy(void *dest, void *src, size_t size) {
    if (size <= 0 || dest == src) {
        return src;
    }


    if ((src < dest && (char *)src + size > (char *)dest) || 
        (dest < src && (char *)dest + size > (char *)src)) {
        // If there's an overlap, copy backwards
        const char *s = (const char *)src + size; // Point to the end of source
        char *d = (char *)dest + size; // Point to the end of destination
        while (size--) {
            *(--d) = *(--s);
        }
    } else {
        // If no overlap, copy forwards
        const char *s = (const char *)src;
        char *d = (char *)dest;
        while (size--) {
            *d++ = *s++;
        }
    }

    return dest;
}