#define		MAIN
#include	"codeconv.h"
#include	"filecntl.h"
#include	"message.h"

#define		MAXL_BUF		128

struct FileControl	outfile = { "", NULL, 0, RECFM_F, 32, 0, 0, 0 };
char tmp[]="test";

char				buf1[MAXL_BUF];
char				buf2[MAXL_BUF];

main(int argc, char *argv[])
{

	_jobname = tmp;
	ascebc_init();
	keijis_init();
	
	code_dmp( "kei2sjis" );

	code_dmp( "ebch2asc" );

}

int		test2(char *fname)
{
	int		len;

	strcpy(outfile.fname, fname);
	f_open(&outfile, "wb");

	strcpy(buf1, "0123456789abcdef");
	hexset(buf1, strlen(buf1), buf2, &len);
	f_writeF(buf2, len, &outfile, 'x');
	
	strcpy(buf1, "0123456789ABCDEF");
	hexset(buf1, strlen(buf1), buf2, &len);
	f_writeF(buf2, len, &outfile, 'X');
	
	strcpy(buf1, "ghijk");
	hexset(buf1, strlen(buf1), buf2, &len);
	f_writeF(buf2, len, &outfile, 'Z');
	
	f_close( &outfile );

	return 0;

}



int		test1(char *fname)
{
int		ret;
int		n;
int		l;

	strcpy(outfile.fname, fname);
	f_open(&outfile, "wb");
	
	strcpy(buf1, "1234567890");
	l = strlen( buf1 );
	asc2ebce(buf1, l, buf2, &n);
	f_writeF(buf2, n , &outfile, ' ');

	strcpy(buf1, "abcdefghijklmnopqrstuvwxyz");
	l = strlen( buf1 );
	asc2ebce(buf1, l, buf2, &n);
	f_writeF(buf2, n, &outfile, ' ');

	strcpy(buf1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	l = strlen( buf1 );
	asc2ebce(buf1, l, buf2, &n);
	f_writeF(buf2, n, &outfile, ' ');

	f_close(&outfile);

	return 0;
}

