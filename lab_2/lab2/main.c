#include <stdio.h>
#include <stdlib.h>

int decimalTG(int n) {
    return n ^ (n >> 1);
}

int grayTD(int gray) {
    int decimal = gray;
    while (gray > 0) {
        gray = gray >> 1;
        decimal = decimal ^ gray;
    }
    return decimal;
}

int main()
{
    int dtg = decimalTG(10);
    printf("dtg: %d",dtg);
    int gtd = grayTD(15);
    printf("\n gtd : %d",gtd);
    return 0;
}
