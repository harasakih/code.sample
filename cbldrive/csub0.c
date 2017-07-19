#include	<stdio.h>

typedef struct {
	int		prm1;
	char	prm2[10];
} t_parm1;


long int	CSUB0(t_parm1 *prm1)
{
	int		i;

	for(i = 0; i < prm1->prm1; i++)
	{
		printf("%c", prm1->prm2[prm1->prm1]);
	}
	printf("\n");

	strcpy(prm1->prm2, "# CSUB0 #");
	prm1->prm1 = prm1->prm1 + 10;
	return(0);
}

