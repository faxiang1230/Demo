#include <stdio.h>  
void my_init(void) __attribute__((constructor)); //告诉gcc把这个函数扔到init section  
void my_fini(void) __attribute__((destructor));  //告诉gcc把这个函数扔到fini section
void out_msg(const char *m)
{  
    printf("    Our_Func \n");  
}  

void my_init(void)  
{  
    printf("\n    Init \n");  
}  
void my_fini(void)  
{  
    printf("    Fini \n");  
}
int main(int argc, char **argv)
{
    printf("    main\n");
}
