#include <stdio.h>
extern int funca();
extern int funcb();
extern int funcc();
extern int funcd();
int main()
{
	printf("funcb:%d funcc:%d\n", funcb(), funcc());
	//printf("funca:%d funcb:%d funcc:%d funcd:%d\n", funca(), funcb(), funcc(), funcd());
	return 0;
}
