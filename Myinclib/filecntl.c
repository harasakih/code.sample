/* ----------------------------------------------------------------------------
ﾌｧｲﾙ	ＦＩＬＥＣＮＴＬ．Ｈの試験用ドライバ
---------------------------------------------------------------------------- */
#define		MAIN

#include	"filecntl.h"
#define		MAXL_BUF		128

struct FileControl	infile  = { "",  NULL, 0, 0, 0, 0, 0 } ;
struct FileControl	outfile = { "outfile", NULL, 0, 0, 0, 0, 0 };
char				buf[MAXL_BUF];

main(int argc, char *argv[])
{

	test5("infile.c05");

}

int		test5(char *fname)
{
int		ret;

	strcpy(infile.fname, fname);
	infile.recfm = RECFM_VB;
	f_open( &infile, "rb" );

	strcpy(outfile.fname, "outfile");
	f_open(&outfile, "wb");

	ret = f_readVB(buf, &infile);
	while( infile.eof != FCNTL_EOF )
	{
		buf[infile.lrecl] = 0x00;
		printf("%d, %d, %d:[%s]\n", infile.blksize, ret, infile.lrecl, buf);
		f_writeT(buf, &outfile);
		ret = f_readVB(buf, &infile);
	}

	f_close( &infile );
	f_close( &outfile );

	printf("cnt=%d\n", infile.iocnt);
	printf("cnt=%d\n", outfile.iocnt);
}


int		test4(char *fname)
{
	int		ret;

	strcpy(infile.fname, fname);
	infile.lrecl = 8;
	f_open( &infile, "rb");

	strcpy(outfile.fname, "outfile");
	f_open( &outfile, "wb");

	ret = f_readF(buf, &infile);
	while( infile.eof != FCNTL_EOF )
	{
		buf[infile.lrecl] = 0x00;
		printf("%d, %d:[%s]\n", ret, infile.lrecl, buf);
		f_writeT(buf, &outfile);
		ret = f_readF(buf, &infile);
	}

	f_close( &infile );
	f_close( &outfile );

	printf("cnt=%d\n", infile.iocnt);
	printf("cnt=%d\n", outfile.iocnt);

}


int		test3(char *fname)
{
	int		ret;

	strcpy(infile.fname, fname);
	infile.lrecl = 16;
	f_open( &infile, "rb");

	ret = f_readF(buf, &infile);
	while( infile.eof != FCNTL_EOF )
	{
		buf[infile.lrecl] = 0x00;
		printf("%d, %d:[%s]\n", ret, infile.lrecl, buf);
		ret = f_readF(buf, &infile);
	}
	printf("cnt=%d", infile.iocnt);
}


int		test2(char *dummy)
{
	int		ret;

	f_open( &infile, "rb");

	ret = f_readV(buf, &infile);
	while(infile.eof != FCNTL_EOF) {
		buf[infile.lrecl] = 0x00;
		printf("%d, %d:[%s]\n", ret, infile.lrecl, buf);
		ret = f_readV(buf, &infile);
	}
	fclose(infile.fpointer);
}


int		test1(char *dummy)
{
	int		ret;
	f_open( &outfile, "wb");

	outfile.lrecl = 16;

	strcpy(buf, "0123456789ABCDEF");

	ret = f_write(buf, 16, &outfile);
	printf("%d, %d\n", ret, outfile.lrecl) ;

	ret = f_writeF(buf,10, &outfile, 'x');
	printf("%d, %d\n", ret, outfile.lrecl);

	ret = f_writeV(buf,12, &outfile);
	printf("%d, %d\n", ret , outfile.lrecl);

	fclose(outfile.fpointer);

	printf("cnt=%d\n", outfile.iocnt);

}
