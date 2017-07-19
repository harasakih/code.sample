/*
	先頭８バイトをメンバ名として，ファイルに分割する
	出力は，０バイト目か８バイト目からのいづれか
	
History
	2000/05/12	ホストファイル名に「¥」が使用可能なので，
				￥⇒＿へ変換
	2000/06/24	￥⇒＿の変換が，先頭１文字だけだっのを，メンバ名全体に修正
*/


#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>

#define		MAXBUF		1024

void	usage(void);

void	usage() {
	fprintf(stderr, "usage: split source-file dest-dir member-yn¥n");
}


int		main(int argc, char *argv[]) {

FILE	*fpIn, *fpOut;
char	buf[MAXBUF];
char	dest_dir[MAXBUF];
char	fn_out[MAXBUF];
char	member[9];
char	old_member[9];
char	yn;
int		l;
char	*chrP;
int		i;

	if(argc != 4) {
		usage();
		exit(-1);
	}
	
	if( (fpIn	=	fopen(argv[1], "r")) == NULL ) {
		fprintf(stderr, "In file error[%s]¥n", argv[1]);
	}
	
	yn	=	toupper( *argv[3] );
	strcpy(dest_dir, argv[2]);
	
/* １レコード先読み */
	strcpy(old_member, "        ");
	fpOut	=	fopen("$", "w");
	while( fgets(buf, MAXBUF, fpIn) != NULL ) {
		sscanf(buf, "%8s", member);
		if( strlen( member ) == 0 ) { continue; }
		for(i = 0; i < 8 && member[i] != '¥0'; i++) {
			if(member[i] == '¥¥') {
				member[i]	=	'_';	/* メンバ名の「￥⇒＿」置換 */
			}
		}
/*
		printf("%s:%d¥n", member,l);
*/
											/* member替わり */
		if( strcmp(old_member, member) != 0) {
				fclose(fpOut);

				strcpy(fn_out, dest_dir);
				strcat(fn_out, "¥¥");
				strcat(fn_out, member);
				strcat(fn_out, ".txt");
/* DBG-Disp */
				fprintf(stderr, "%s¥n", fn_out);

				if( (fpOut	=	fopen(fn_out, "w")) == NULL ) {
					fprintf(stderr, "Out file error[%s/%s]¥n", argv[2], member);
				}
				strcpy(old_member, member);
		}
		if(yn == 'Y') {
			fputs(buf, fpOut);
		} else {
			fputs(buf+9, fpOut);
		}
	}

fclose(fpOut);
fclose(fpIn);

return	0;

}
