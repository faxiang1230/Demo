#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int pi[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
static int tmp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int tmp3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int s[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float avg[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//down -> up
int down_to_up(int len)
{
	int i = 0, j = 0, max = 0, max1 = 0;

	for (i = 1; i <= len; i++) {		//从最小子问题开始求解，逐步逼近问题
		for(j = 1, max = pi[i]; j < i; j++) {	//初始最大值为不切割
			max1 = tmp3[i - j] + tmp3[j];	//不同的切割状态
			if (max1 > max) {
				max = max1;
				s[i] = i- j;
			}
		}
		tmp3[i] = max;	//更新最后的最大值
	}

	return tmp3[len];
}
//up -> down
int up_to_down(int len)
{
	int i = 0, max = pi[len], max1 = 0, first = 0;
	if (tmp[len])
		return tmp[len];
	for(i = 1; i <= len; i++) {
		max1 = up_to_down(len - i) + tmp[i];
		if (max1 > max) {
			max = max1;
			first = len - i;
		}
	}
	tmp[len] = max;
	s[len] = first;
	return max;
}
//simple recursive
int simple_recursive(int len)
{
	int i = 0, max = pi[len], max1 = 0;
	for(i = 1; i <= len; i++) {
		max1 = simple_recursive(len - i) + pi[i];
		if (max1 > max) {
			max = max1;
		}
	}
	return max;
}
int main(int argc, char **argv)
{
	int len = 0, index = 0;
	if (argc != 2) {
		printf("Usage:	%s len\n", argv[0]);
		_exit(0);
	}
	
	len = strtol(argv[1], NULL, 0);
	printf("simple_recursive len:%d max %d\n", len, simple_recursive(len));
	memset(s, 0, sizeof(s));
	printf("up_to_down len:%d max %d\n", len, up_to_down(len));
	for (index = 1; index <= len; index++) {
		printf("%d ", tmp[index]);
	}
	printf("\n");
	printf("cut:\n");
	for (index = 1; index <= len; index++) {
		printf("%d ", s[index]);
	}
	printf("\n");
	memset(s, 0, sizeof(s));
	printf("down_to_up len:%d max %d\n", len, down_to_up(len));
	for (index = 1; index <= len; index++) {
		printf("%d ", tmp3[index]);
	}
	printf("\n");
	printf("cut:\n");
	printf("down_to_up len:%d max %d\n", len, down_to_up(len));
	for (index = 1; index <= len; index++) {
		printf("%d ", s[index]);
	}
	printf("\n");
	return 0;
}
