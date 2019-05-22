#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <assert.h>

int main(int argc, char *argv[]) {
	DIR *dp = opendir(".");
	assert(dp != NULL);
	struct dirent *d;
	while ((d = readdir(dp)) != NULL) {
		printf("%d %s\n", (int) d->d_ino, d->d_name);
	}
	closedir(dp);
	return 0;
}
