#include<stdio.h>
void mian()
{
  int n,rev,rem;

  printf("enter a number: ");
  scanf("%d",&n);
   
   while(n !=0)
   {
    rem=n%10;
    rev=rev*10+rem;
    n/=10;
   }
   printf("%d",rev);
}