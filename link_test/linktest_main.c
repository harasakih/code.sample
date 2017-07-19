#include	<stdio.h>

extern	char*	gsStr1;
static	char*	gsStr2="gsStr2";


int	main(int argc, char **argv)
{

	printf("in main 1 %s\n", gsStr1);
	printf("in main 2 %s\n", gsStr2);

	sub1("call sub1 from main\n");
	sub2("call sub2 from main\n");
}
