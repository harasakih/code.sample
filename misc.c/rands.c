#include	<stdio.h>
#include	<stdlib.h>
#define		MAX_ARRAY	4096	

void	init_array(int *a, int kosu);
void	disp_array(int *a, int kosu);


int	main(int argc, char **argv)
{
	int		array[MAX_ARRAY];
	int		kosu;

	if( argc != 2 ) {
		printf("usage: %s kosu_of_rand \n", argv[0]);
		exit( 1 );
	}

	kosu	=	atoi( argv[1] );
	if( kosu > MAX_ARRAY ) {
		printf("kosu_of_rand exceed %d, set lower\n", MAX_ARRAY);
		exit( 1 );
	}

	init_array( array, kosu );
	disp_array( array, kosu );
}

void	init_array( int *array, int narray )
{
	int	tmp;
	int	i,j;
	int	ck;		/* 1:ok 0:ng */

	srand( 97 );
	array[0]	=	rand()%narray ;

	for(i = 1; i < narray; i++) {
		do {
			tmp	=	rand()%narray ;
			for(j = 0; j < i; j++) {
				if( array[j] != tmp) {
					ck	=	1;
				} else {
					ck	=	0;
					break;
				}
			}
		} while( ck == 0 ) ;
		array[i]	=	tmp;
	}
}

void	disp_array( int *array, int narray )
{
	int	i;

	for(i = 0; i < narray; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}
