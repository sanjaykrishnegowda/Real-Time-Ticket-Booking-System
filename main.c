#include<stdio.h>
int reverse( char str[],int n)
{
   for(int i=0;i<n/2;i++)
   {
        char temp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = temp;
   }
printf("%s",str);
}
int main()
{
 char str[100]="Shashank";
 int i;
 while(str[i]!='\0')
 {
    i++;
 }
 reverse(str,i);
}