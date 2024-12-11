#include<stdio.h>
#include<math.h>
void main()
{
  int  a[6],varience,sd,i;
  float sum=0,mean;
  printf("enter the 6 numbers: ");
  for(i=0;i<6;i++)
  {
  scanf("%d",&a[i]);
  }
  for(i=0;i<6;i++){
  sum=sum+a[i];}
  mean=(float)sum/6;
  
  printf("the mean of given numbers is: %.2f\n",mean);

}
