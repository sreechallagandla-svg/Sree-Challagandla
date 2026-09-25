#include<stdio.h>
void Toh(int n,char source ,char Aux,char dest)
{
if(n==1)
{
printf("move 1 from %c to %c", source , dest);
}
Toh(n-1,source,dest,Aux);
printf("move %d from %c to %c", n, source, dest);
}
int main() {
int a;
printf("Enter no.of disk");
scanf("%d",&a); 
Toh(a,'A','B','C');
return 0;
}
