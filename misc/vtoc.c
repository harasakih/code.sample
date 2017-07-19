/*
	ARCSのVTOCﾘｽﾄを編集し，WRN-MSG（容量監視）を出力する
*/

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#define		MAXBUF		1024
#define		MAXBUF2		128

void	usage() {
	fprintf(stderr, "usage : vtoc in-file out-file¥n");
}

void	main(int argc, char *argv[]) {

FILE	*fpIn, *fpOut;
char	buf[MAXBUF];
char	volser[7];
char	wrn[6];
char	dsn[MAXBUF2];
char	dsorg[MAXBUF2];
char	recfm[MAXBUF2];
char	create[MAXBUF2];
char	refer[MAXBUF2];
int		lrecl;
int		blksize;
int		total;
int		empty;
int		ext;

	if(argc != 3) {
		usage();
		exit(-1);
	}

	if( (fpIn	=	fopen(argv[1], "r")) == NULL ) {
		fprintf(stderr, "In file error[%s]¥n", argv[1]);
	}

	if( (fpOut	=	fopen(argv[2], "w")) == NULL ) {
		fprintf(stderr, "Out file error[%s]¥n", argv[2]);
	}

	while( fgets(buf, MAXBUF, fpIn) != NULL ) {
		if( strncmp(buf+7, "*** VOLUME SUMMARY OF", 21) == 0 ) {
			strncpy(volser, buf+29, 6);
			volser[6]	=	'¥0';
			fprintf(fpOut, "%-7s%-40s%-4s%-2s%6s%6s%6s%6s%6s %-10s %-10s %-5s¥n",
				"volser", "----------dsn----------", "org", "fm", "lrecl", "blksz", "total", "empty", "ext", "create", "refer", "wrn");
		} else {
			sscanf(buf, "%s%s%s%d%d%d%d%d%s%s", 
				dsn, dsorg, recfm, &lrecl, &blksize, &total, &empty, &ext, create, refer);
			strcpy(wrn, "     ");
			if(ext > 8)  { strcpy(wrn, "*"); }
			if(ext > 12) { strcpy(wrn, "**"); }
			if(ext > 15) { strcpy(wrn, "***"); }
			if(ext == 16 && total == empty) { strcpy(wrn, "*****"); }
			
			fprintf(fpOut, "%-7s%-40s%-4s%-2s%6d%6d%6d%6d%6d %-10s %-10s %-5s¥n",
				volser, dsn, dsorg, recfm, lrecl, blksize, total, empty, ext, create, refer, wrn);
		}
	}


	fclose(fpIn);
	fclose(fpOut);
}
