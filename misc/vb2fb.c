/*
 *	vb2fb
 *
 *	日報試験データ作成のためのツール
 *	ＶＢ（先頭４Ｂレコード長）をＦＢに変換
 */
 
#define		MAIN
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<memory.h>
#include	"filecntl.h"

#define		MAX_BUF		2048

void	usage();

struct	FileControl	infile;
struct	FileControl	outfile;

int	main(int argc, char *argv[])
{
	int		i;
	char	*opt;
	char	buf[MAX_BUF];
	
	if(argc != 4) { 
		usage();
		exit(-1);
	}
	
	for(i = 1; i < argc; i++) {
		opt = argv[i] +1;
		switch( *opt ){
		case 'l':
			outfile.lrecl = atoi( opt + 1 );
			break;
		case 'i':
			strcpy(infile.fname, opt + 1);
			infile.recfm = RECFM_V;
			break;
		case 'o':
			strcpy(outfile.fname, opt + 1);
			outfile.recfm = RECFM_F;
			break;
		default:
			usage();
			exit(-1);
		}
	}
	
	if(f_open(&infile, "rb")) { 
		printf("infile open error [%s]\n", infile.fname);
		exit(-1);
	}
	
	if(f_open(&outfile, "wb")) {
		printf("outfile open error [%]\n", outfile.fname);
		exit(-1);
	}
	
	memset(buf, (char)0x00, MAX_BUF);
	while(readV(buf, &infile), infile.eof != FCNTL_EOF) {
		f_writeF(buf, outfile.lrecl, &outfile, 0x00);
		memset(buf, (char)0x00, MAX_BUF);
	}
	
	f_close(&infile);
	f_close(&outfile);
}

void	usage(){
	puts("vb2fb -l(outlength<2048) -i(infile) -o(outfile)");
}

int		readV(char *buf, struct FileControl *fcntl)
{
	char		rdw[RDW_LENGTH];
	int			lrecl;
	union	{
		char	c[2];
		short	x;
	} ll;
	
/*
	assert( sizeof(short) == 2  );
*/
	/*
	 *	RDW部を読んでレコード長を算出
	 */
	fread(rdw, RDW_LENGTH, 1, fcntl->fpointer);
	ll.c[0] = rdw[1];
	ll.c[1] = rdw[0];

	memcpy(buf, rdw, RDW_LENGTH);

	/*
	 *	EOFならすぐ戻る。
	 */
	if( feof(fcntl->fpointer) ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		/*
		 *	実際のレコード長は４バイト短くなる
		 */
		lrecl = ll.x - RDW_LENGTH;
/*
		assert( lrecl >= 0 );
*/
		fread(buf+4, lrecl, 1, fcntl->fpointer);
		fcntl->lrecl = ll.x;
		fcntl->iocnt++;
		return fcntl->lrecl;
	}
}
