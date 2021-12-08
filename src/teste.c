#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int p;

int factorial (int n, int x) {
    int abc = n + x + 3;
    return abc;
}


int main () {
    bool a = 0;
    bool b = 1;
    bool c, d;

    c = a && b;
    d = a || b;    

    factorial(5, 6);

    return(0);
}