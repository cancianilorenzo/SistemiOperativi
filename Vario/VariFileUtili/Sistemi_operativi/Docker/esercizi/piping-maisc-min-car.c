#include <stdio.h>
int main() {
    int c, d;
    // loop into stdin until EOF (as CTRL+D)
    while ((c = getchar()) != EOF) { // read from stdin
        d = c;
        if (c >= 'a' && c <= 'z') d -= 32;
        if (c >= 'A' && c <= 'Z') d += 32;
        putchar(d);                  // write to stdout
};
return (0); }