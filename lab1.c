//Q. write a c program to reverse the value of five integers variable using an array of pointers.
#include<stdio.h>
int main() {
    int a, b, c, d, e;
    int *ptr[5];
    printf("Enter five integers:\n");
    scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);

    ptr[0] = &a;
    ptr[1] = &b;
    
    ptr[2] = &c;
    ptr[3] = &d;
    ptr[4] = &e;

    for (int i = 0; i < 5 / 2; i++) {
        int temp = *ptr[i];
        *ptr[i] = *ptr[4 - i];
        *ptr[4 - i] = temp;
    }
    printf("Reversed values:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", *ptr[i]);
    }
    printf("\n");

    return 0;
}