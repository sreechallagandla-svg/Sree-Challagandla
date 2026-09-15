#include<stdio.h>
int main() {
    int x=10;
    int* p=&x;
    *p =50;
printf("%d",x);
    return 0;   
}