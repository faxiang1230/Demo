/*************************************************************************
    > File Name: strstr.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mystrstr(char *str, char *needle)
{
	int needle_len = strlen(needle);
	int str_len = strlen(str);
	int index = 0;
	if (needle_len > str_len) return NULL;
	if (strncmp(str, needle, needle_len) == 0) return str;
	for(index = 0; index < str_len - needle_len; index++) {
		if(strncmp(str+index, needle, needle_len) == 0) {
			return str+index;
		}
	}
	return NULL;
}
int main(int argc, char **argv)
{
	char *str1 = "nihao3";
	char *str2 = "hao";
	printf("%s\n", mystrstr(str1, str2));	
	return 0;
}
