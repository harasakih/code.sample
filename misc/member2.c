#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>

#define		MAXBUF		1024

void	usage(void);


void	usage() {
	fprintf(stderr, "usage: member¥n");
	fprintf(stderr, "  lib,member,memberﾒｲ-ﾋｮｳｼﾞ ｦ ﾄｲｱﾜｾ¥n");
}


int		main(int argc, char *argv[]) {

FILE	*fpIn, *fpOut;
char	buf[MAXBUF];
char	fnOut[MAXBUF];
char	fnIn[MAXBUF];
char	member[MAXBUF];
char	dest[MAXBUF];
int		l;
char	yn, tmp;
int		flg;

	if(argc != 1) {
		usage();
		exit(-1);
	}
	
	printf("member Ver.1.0 2000/05 Copyright Hiroshi Harasaki¥n");
	printf("  BMUPO2PSの出力などから，先頭８バイト指定で抽出¥n");
	
	printf("¥n入力ファイル名        ==> ");
	scanf("%s", fnIn);
	
	printf("出力ディレクトリ(.¥¥/?)==> ");
	scanf("%s", dest);

	yn		=	'Y';
	while( yn	==	'Y') {
		if( (fpIn	=	fopen(fnIn, "r")) == NULL ) {
			fprintf(stderr, "In file error[%s]¥n", fnIn);
			exit(-1);
		}
	
		printf("¥n");
		printf("メンバ名(先頭８バイト) ==> ");
		scanf("%s",		member);
		printf("メンバ名付加(Y/N)      ==> ");
		scanf("%s",		buf);
		l	=	strlen( member );
		yn	=	toupper( buf[0] );

		strcpy(fnOut, dest);
		strcat(fnOut, member);
		strcat(fnOut, ".txt");
		if( (fpOut	=	fopen(fnOut, "w")) == NULL ) {
			fprintf(stderr, "Out file error[%s]¥n", fnOut);
		}
		/* 指定ファイルの処理 */
		flg	=	0;
		while( fgets(buf, MAXBUF, fpIn) != NULL && flg != 2) {
/*
			printf("%d:%s¥n", flg, buf);
*/
			if( strncmp(member, buf, l) == 0 ) {
				if(yn == 'Y') {
					fputs(buf, fpOut);
				} else {
					fputs(buf+9, fpOut);
				}
										/* ﾒﾝﾊﾞ名が一致した時 */
				switch( flg ) {
				case	0:
					flg	=	1;			/* 初めてだったら，１にする			*/
					break;
				case	1:
					flg	=	1;			/* 一回ヒットしていたら，そのまま１	*/
					break;
				deault:
					break;
				}
			} else {
										/* ﾒﾝﾊﾞ名が不一致の時 */
				switch( flg ) {
				case	0:
					flg	=	0;			/* まだ，指定メンバに行きついていない	*/
					break;
				case	1:				/* 指定メンバの後，メンバ替わり			*/
					flg	=	2;			/* 入出力ファイルをクローズ				*/
					break;
				default:
					break;
				}
			}
		}
		
		fclose(fpIn);
		fclose(fpOut);
		
		printf("次の処理(Y/N)          ==> ");
		scanf("%s",		buf);
		yn	=	toupper( buf[0] );
	}
	
	return 0;
}
