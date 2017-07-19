#include	"utility.c"

main( argc, argv)
  int	argc;
  char	*argv[];
{
int	c;
char	*options = "A:F:PuU:wXZ:";

	while((c = getopt( argc, argv, options )) != EOF ){
		printf("recognized options : %c\n", c);
		printf("option argument    : %s\n", optarg);
	}
	while( argc-- > 0 )
	    printf("argv[%d] : %s\n", argc,argv[argc] );

}
