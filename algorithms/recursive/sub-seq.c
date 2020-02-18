//string's all sub sequence
#include <stdio.h>
#include <string.h>
char str[128] = "abcdefg";
char res[128] = {0};

void sub_seq(int str_offset, char *string, int str_len, int res_offset, char *result)
{
	int i = str_offset;
	if (str_offset == str_len) {
		printf("result:%s\n", result);
		return;
	}

	result[res_offset] = 0;
	sub_seq(i + 1, string, str_len, res_offset, result);

	result[res_offset] = string[i];
	result[res_offset + 1] = 0;
	sub_seq(i + 1, string, str_len, res_offset + 1, result);
}
int main(int argc, char **argv)
{
	sub_seq(0, str, strlen(str), 0, res);
	return 0;
}
