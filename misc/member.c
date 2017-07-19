/*
    先頭８バイトをメンバ名として，指定メンバーを抽出する
    出力は，０バイト目か８バイト目からのいづれか

   History
    2000/05/12	ホストファイル名に「\」が使用可能なので，
                ￥⇒＿へ変換
    2000/06/24	￥⇒＿の変換が，先頭１文字だけだっのを，メンバ名全体に修正
 */


#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>

#define     MAXBUF      1024

void    usage(void);

void    usage() {
        fprintf(stderr, "usage: member source-file member\n");
}


int     main(int argc, char *argv[]) {

        FILE    *fpIn;
        char buf[MAXBUF];
        char member[9];
        char member_arg[9];
        int l;
        int i;
        int member_find;

        if(argc != 3) {
                usage();
                exit(-1);
        }

        if( (fpIn   =   fopen(argv[1], "r")) == NULL ) {
                fprintf(stderr, "In file error[%s]\n", argv[1]);
        }


        strcpy(member_arg, argv[2]);


        member_find = 0;
        while( fgets(buf, MAXBUF, fpIn) != NULL ) {
                sscanf(buf, "%8s", member);
                if( strlen( member ) == 0 ) { continue; }
                for(i = 0; i < 8 && member[i] != '\0'; i++) {
                        if(member[i] == '\\') {
                                member[i]   =   '_';/* メンバ名の「￥⇒＿」置換 */
                        }
                }
/*
        printf("%s:%d\n", member,l);
 */
                if( strcmp(member_arg, member) == 0 ) { /* member find */
                        member_find = 1;
                        fputs(buf+9, stdout);
                } else {
                        if(member_find == 1) {
                                exit(-1);
                        } else {
                                ;
                        }
                }
        }

        fclose(fpIn);

        return 0;

}
