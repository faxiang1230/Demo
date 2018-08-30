#include <stdio.h>
void b(int x) {
	char buf[16];
	sprintf(buf, "bbbbbb%d\n", x);
	write(1, buf, strlen(buf));
}
void a() {
	write(1, "aaaaa\n", 6);
	b(1);
	b(1);
}
void main() {
	a();
}
