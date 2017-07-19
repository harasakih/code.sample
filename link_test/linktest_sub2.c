#include	<stdio.h>

char	*gsStr1="gsStr1 sub2";

int	sub1(char *s)
{
	printf("  in sub1\n");
	return 1;
}

int	sub2(char *s)
{
	printf("  in sub2\n");
	printf("  in sub2\n");
	return 2;
}
