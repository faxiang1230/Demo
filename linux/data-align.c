#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma pack(2)
typedef struct dataalign{
    char a;
    double b;
    char c;
}data1;
//}__attribute__((__packed__())) data1;

void main()
{
        printf("sizeof :%d\n", sizeof(data1));
}
