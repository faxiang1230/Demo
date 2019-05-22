//blog.chinaunix.net/uid-29482215-id-4120748.html
#include <stdio.h>
#include <sys/prctl.h>

int main(int argc, char *argv[], char *envp[])
{
    char *new_name = "abcdefghijklmnopqrstuvwxyz";

    getchar();
    prctl(PR_SET_NAME, new_name);
    getchar();

    return 0;
}


