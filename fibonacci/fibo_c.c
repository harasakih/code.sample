#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>

/* proto-type def */
unsigned long	fibo(int n);

unsigned long	fibo(int n)
{
	unsigned long	f;

	if( n == 0) {
		f	=	0;
	} else if( n == 1) {
		f	=	1;
	} else {
		f	=	fibo(n - 1) + fibo(n - 2);
	}
	return(f);
}

int	main(int argc, char **argv)
{
	int				i,n;
	unsigned long	f;
	clock_t			t1,t2;
	double			diff;

	n	=	atoi( argv[1] );
	for(i = 1; i <= n; i++) {
		t1	=	clock();
		f	=	fibo(i);
		t2	=	clock();
		diff	=	(double)(t2 -  t1)/CLOCKS_PER_SEC;
		printf("Fibo(c_r) %d time %f %lu\n", i, diff, f);
	}
}

