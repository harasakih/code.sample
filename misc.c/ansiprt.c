/*
 *	ansiprt		Copyright 1997,Hiroshi Harasaki
 *
 *		JOBLOGﾌｧｲﾙを、ANSI制御文字に従って印刷制御する。
 *
 *	ANSI制御ｷｬﾗｸﾀ
 *		空白	印刷前に１行改行
 *		０		印刷前に２行改行
 *		−		印刷前に３行改行
 *		＋		行送りしない（重ね打ち）
 *		１		印刷前に改ページ
 *		２		チャネル２にスキップ
 *		．．．
 *		Ｃ		チャネル１２にスキップ
 *
 *	Ver.0.0L00	1997.08.15	作成開始
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#define     Max_Buf     256

void        usage();

main(int argc, char *argv[])
{
        FILE            *fp_infile;
        FILE            *fp_outfile;
        char buf[Max_Buf];
        char ansi_char;
        char            *data;
        int l;
/*
 *	ＡＲＧのチェック
 */
        if(argc != 3) {
                usage();
                exit(-1);
        }

/*
 *	入力ファイルのオープン
 */
        if( (fp_infile = fopen(argv[1], "rt")) == NULL ) {
                printf("Infile error\n");
                exit(-1);
        }

/*
 *	出力ファイルのオープン
 */
        if( (fp_outfile = fopen(argv[2], "wt")) == NULL ) {
                printf("Outfile error\n");
                exit(-1);
        }

        while( fgets(buf, Max_Buf, fp_infile) != NULL ) {
                /*
                 * fgetsはbufに\nを含み最後に\0が付く
                 * →\nの位置に\0を置く(getsに似せる)
                 */
                l = strlen(buf);
                buf[l - 1] = 0x00;

                ansi_char = buf[0];
                data      = &buf[1];

                switch( ansi_char ) {
                case    ' ':
                        fprintf(fp_outfile, "\n%s", data);
                        continue;
                case    '0':
                        fprintf(fp_outfile, "\n\n%s", data);
                        continue;
                case    '-':
                        fprintf(fp_outfile, "\n\n\n%s", data);
                        continue;
                case    '+':
                        fprintf(fp_outfile, "%s", data);
                        continue;
                case    '1':
                        fprintf(fp_outfile, "\n\f%s", data);
                        continue;
                default:
                        fprintf(fp_outfile, "\n%s", &buf[0]);
                        continue;
                }
        }
}

void    usage()
{
        printf("ansiprt Ver.0.0-L00 Copyright 1997, Hiroshi Harasaki\n");
        printf("  ANSI印刷制御文字に従って、改行・改頁制御をする。\n");
        printf("\n");
        printf("usage : ansiprt infile outfile\n");
}
