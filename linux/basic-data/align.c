/*************************************************************************
    > File Name: align.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct placeholder {
	int a;
	char b[0];
};

/* 
 * It is important to note that the last member is padded with the number of bytes
 * required so that the total size of the structure should be a multiple of the
 * largest alignment of any structure member
*/
struct padding1 {
	short a;
	char b[3];
};
struct padding {
	double a;
	char b;
};
struct bitfield {
	char a:3;
	char b:5;
};
void main() {
	struct bitfield bf;
	printf("sizeof placeholder:%lu\n", sizeof(struct placeholder));
	printf("sizeof padding1:%lu\n", sizeof(struct padding1));
	printf("sizeof padding:%lu\n", sizeof(struct padding));
	bf.a = 3;
	bf.b = 0x12;
	printf("bf:0x%x\n", *((unsigned char*)&bf));
}
