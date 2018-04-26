void main() {
	void *x = malloc(0);
	void *y = malloc(0);
	printf("malloc X addr:%p\n", x);
	printf("malloc Y addr:%p\n", y);
	free(x);
	free(y);
	printf("free X %p\n", x);
	printf("free Y %p\n", y);
}
