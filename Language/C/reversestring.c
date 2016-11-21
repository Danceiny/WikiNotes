/*************************************************************************
    > File Name: reversestring.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun, Oct 30, 2016  2:41:31 PM
 ************************************************************************/

#include<stdio.h>
void print()
{
	char a;
	scanf("%c",&a);
	if(a!='#')print();
	if(a!='#')printf("%c",a);
}
int main()
{
	print();
	return 0;
}
