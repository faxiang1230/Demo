/*************************************************************************
    > File Name: test.c
    > Author: wangjx
 ************************************************************************/
#include "sms4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
	sms4_key_t key, key2;
	int i = 0;
	unsigned char origin[16] = {
		0xeb, 0x76, 0x90, 0x45, 0x58, 0x46, 0x41, 0x54,
	  	0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
#if 0
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                            
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,  
#endif
	};
	unsigned char user_key[16] = {
		0xe8, 0xcf, 0xa3, 0xdb, 0xfe, 0x37, 0x3b, 0x53,
		0x6b, 0xe4, 0x3c, 0x56, 0x37, 0x38, 0x77, 0x86,
#if 0
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
#endif
	};
	unsigned char *out = malloc(16);
	memset(out, 0, 16);
	unsigned char *out2 = malloc(16);
	memset(out2, 0, 16);
	sms4_set_encrypt_key(&key, user_key);
	printf("plaintext:\n\t");
	for (i = 0; i < 16; i++)
		printf("%02x ", origin[i]);
	printf("\n");
	sms4_encrypt(origin, out, &key);
	printf("encrypt:\n\t");
	for (i = 0; i < 16; i++)
		printf("%02x ", out[i]);
	printf("\n");

	sms4_set_decrypt_key(&key2, user_key);
	sms4_decrypt(out, out2, &key2);
	printf("decrypt:\n\t");
	for (i = 0; i < 16; i++)
		printf("%02x ", out2[i]);
	printf("\n");
}
