/*************************************************************************
  > File Name: regex.c
  > Author: wangjx
 ************************************************************************/
#include <regex.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>

const int times = 1;

int main(int argc,char** argv)
{
	char pattern[32] = ".swp$";
	const size_t nmatch = 10;
	regmatch_t pm[10];
	int z ;
	regex_t reg;
	char lbuf[256]="set",rbuf[256];
	char buf[3][32] = {"/home/linux/test.sw", "/home/.swpxxx", "/home/linux/test.swp"};
	printf("input strings:\n");
	regcomp(&reg,pattern,REG_EXTENDED|REG_NOSUB);
	for(int i = 0 ; i < times; ++i)
	{
		for(int j = 0 ; j < 3; ++j)
		{
			z = regexec(&reg,buf[j],nmatch,pm,REG_NOTBOL);
			if (z == REG_NOMATCH)
				printf("no match\n");
			else
				printf("match\n");
		}
	}
	return 0 ;
}
