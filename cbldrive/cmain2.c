#include	<stdio.h>
#include	<stdlib.h>

int	main(int argc, char **argv)
{
	typedef	struct
	{
		short	length;
		char	msg[10];
	} t_parm1;
	
	extern	void			JMPCINT2(), JMPCINT3();
	extern	long	int		COBSUB(t_parm1 *parm1);
	
	t_parm1		parm1;
	
	JMPCINT2();
	
	parm1.length	=	strlen( argv[1] );
	strcpy(parm1.msg, argv[1]);
	if( COBSUB( &parm1 ) == 0 )
	{
		printf("cobol call success\n");
	}
	else
	{
		printf("cobol call failure\n");
	}
	
	JMPCINT3();
	return(0);
}
