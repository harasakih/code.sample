/*
 ***********************************************************************
 *	$Id: btdtcnv.c,v 1.4 2006/07/17 07:25:29 hiroshi Exp $
 ***********************************************************************
 *	$Log: btdtcnv.c,v $
 *	Revision 1.4  2006/07/17 07:25:29  hiroshi
 *	*** empty log message ***
 *	
 *	Revision 1.3  2006/06/25 16:04:11  hiroshi
 *	可変長ファイルはホスト／ネットＣＯＢの両方を対応。
 *	
 *	Revision 1.2  2006/06/25 13:25:17  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 * 処理名	：	btdtcnv
 * 機能名称 ：	ホスト可変長ファイルの、コード変換・ＵＮＰＡＣＫ・ＦＭＴ−ＤＭＰ
 * 機能概要 ：	ＪＣＬファイル（環境設定）とＭＡＰファイル（レコードフォーマット）から動作する。
 * 制限事項 ：	
 	(1)linuxでlargeファイルを扱う
		% gcc hoge.c -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 

	(2)gccで外部参照エラー（ＣＰＰの時）
		/tmp/ccuhaV3q.o(.eh_frame+0x11): undefined reference to `__gxx_personality_v0' 
		collect2: ld はステータス 1 で終了しました
		% gcc btdtcnv.cpp -lstdc++

	(3)solarisでlargefileを扱う
		% gcc -DSOLARIS btdtcnv.c
		(fopen 命令を fopen64 命令に変更）

 * 呼出形式 ：	btdtcnv -j[jcl-file] -m[map-file] -l[log-file] -i[infile] -o[outfile]
 * 引数		：	
 	jcl-file	入力・出力のファイル形式や、コード変換ＴＢＬ名などを定義
	map-file	入力ファイルのレコードフォーマットを定義
 	log-file	ＪＯＢＬＯＧ
 	infile		入力ファイル
 	outfile		出力ファイル
 * 返却値	：	0 正常
			　≠0 異常終了
 * 修正履歴	：	2005/10/11	Ver.0.0.A	初版作成。
					レコードＩＤ規定外時にゴミレコード出力してします。
				2006/07/15	ZDに対応（UNPACK､FMTDMPのみ）。CODECONVはレコード長が変わるので止めた

 * 既知バグ	：
 */

#define		MAIN
/*
#define		_LARGEFILE_SOURCE
#define		_FILE_OFFSET_BITS=64
*/

#include	"btdtcnv.h"
#include	"cfgopts.h"
#include	"filecntl.h"
#include	"message.h"
#include	"codecnv2.h"
#include	<ctype.h>
#include	<assert.h>
#include	<stdlib.h>

#define		MYCODE_HAN		CODE_ASC
#define		MYCODE_ZEN		CODE_SJIS




int 	usage(void);
int		uabend(FILE *fp, char *pgmid, int err_no, char *msg, int retcd);
int 	arg_chk(struct CmdLine opt[]);
int		recordid_chkset(char *buf, struct Record_ID *r);
int		get_recid(unsigned char *buf, struct Record_ID r[]);
int		btdtcnv_codeconv(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[]);
int		btdtcnv_fmtdmp(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[]);
int		btdtcnv_unpack(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[], char delimiter);
int		fmtprint(unsigned char *otbuf, struct OtItem otitem[], FILE *fp, int ll);
int		fcntl_fdmp(struct FileControl *fcntl, char *msg, FILE *fp);
int		codeconvctl_fdmp(struct CodeConvCTL ctl, char *msg, FILE *fp);


void	ctl_init_01(int argc, char *argv[]);
void	ctl_init_02(void);
void	ctl_init_03(void);
void	ctl_term_01(void);

struct	CodeConvCTL	dmy_codecntl = {
	"hantbl.csv",
	"zentbl.csv",
	0, 0, 0, 0
} ;


int		main(int argc, char *argv[])
{
	int		l;
	unsigned char	buf[MAX_READBUF];
	unsigned char	otbuf[MAX_WRITEBUF];
	char			msgbuf[256];
	int		rec_id;
	int		recno;
	int		flg;

	if( argc <= 1 ) { usage(); exit(0); }
    if( argc <= 2 ) { 
    	hantbl_init(dmy_codecntl);
    	zentbl_init(dmy_codecntl);
    	code_dmp2( argv[1]+2 );
    	exit(0);
    }

	assert( sizeof(short) == 2  );

	ctl_init_01(argc, argv);			/* 開始ＭＳＧ（ＣＳＬ）、引数チェック			*/
										/* ファイルオープン（ＪＣＬ、ＭＡＰ，ＬＯＧ）	*/
										
	ctl_init_02();						/* ＪＣＬファイルの処理							*/
										/* コード変換ＴＢＬの初期化						*/

	ctl_init_03();						/* ＭＡＰファイルの処理							*/

	fcntl_fdmp(&infile, "[infile]",   logfile.fpointer);
	fcntl_fdmp(&outfile, "[outfile]", logfile.fpointer);
	codeconvctl_fdmp(codeconvctl, "[codetbl]", logfile.fpointer);

#ifdef	DBG_CODETBL
	code_dmp2("HANTBL");
	code_dmp2("ZENTBL");
#endif


/* ****************************************************************************
    主処理
        入出力ファイルファイルオープン
        ログファイルへ開始ＭＳＧの書込み
        変換処理
**************************************************************************** */

	recno	=	0;
	flg		=	0;
	while( f_readANY(buf, &infile) != FREAD_EOF && flg == 0) 
	{
		recno++;
		if((giMaxincount > 0) && (recno > giMaxincount)){
			flg	= 1;
			sprintf(msgbuf, "btdtcnv ended by maxicount[%d]", giMaxincount);
			fil_msg(logfile.fpointer, msgbuf);
			break;								/* while-loop-out 	*/
		}
		if( (rec_id = get_recid(buf, record_id)) < 0) { continue; }		/* Next loop				*/
																		/* １万件ごとのＬＡＰ出力	*/
		if((recno % 10000) == 0) { printf("%d recid=%d\n", recno/10000, rec_id); }
		switch( giExecute ) {
			case EXE_CODECONV:
				l = btdtcnv_codeconv(buf, item[rec_id], otbuf, otitem[rec_id]);
#ifdef DEBUG
				printf("main:recid=%d\n", rec_id);
				printf("main:btdtcnv_codeconv : ret=%d\n", l);			/* codeconvの返却値は、出力レコード長 */
#endif
				f_writeANY(otbuf, l, &outfile, '\0');
				break;
			case EXE_UNPACK:
				l = btdtcnv_unpack(buf, item[rec_id], otbuf, otitem[rec_id], outfile.delimiter);
																				/* buf -> otbuf へUNPACK & CSV化	*/
				f_writeANY(otbuf, l, &outfile, gChar_Space[got_CodeHan]);		/* 出力長が設定されていること 		*/
				break;
			case EXE_FMTDMP:
				l = btdtcnv_fmtdmp(buf, item[rec_id], otbuf, otitem[rec_id]);

				fprintf(outfile.fpointer, ">>>>> BTDTCNV-FMTPRINT : RecNo=%d : RecordID=%d : Infile[%s] : Otfile[%s] <<<<<\n",
										 recno, rec_id, infile.fname, outfile.fname);
				outfile.iocnt++;

				l = fmtprint(otbuf, otitem[rec_id], outfile.fpointer, giMaxFmtprint);
				fprintf(outfile.fpointer, "\n");

				break;
			default:
				break;
		}
	}

/* ****************************************************************************
    終了処理理
        入出力ファイルファイルクローズ
        ログファイルへ終了ＭＳＧの書込み
**************************************************************************** */
	ctl_term_01();
	return 0;

}

/* ----------------------------------------------------------------------------
処理
戻値    
引数
注意
---------------------------------------------------------------------------- */
int usage()
{
    int             i;
    i = 0;
    while( gsVersion[i][0] != '/') {
        puts(gsVersion[i]);
        i++;
    }
    return 0;
}

/* ----------------------------------------------------------------------------
処理	ユーザアベンド
		ＣＳＬとログにＭＳＧを出力し、アベンド
戻値
引数
注意
---------------------------------------------------------------------------- */
int		uabend(FILE *fp, char *pgmid, int err_no, char *msg, int retcd)
{
	char	buf[128];

	sprintf(buf, "pgm=%s:err=%d:%s:rtcd=%d", pgmid, err_no, msg, retcd);
    csl_msgb(buf);
	if(fp != NULL) { fil_msgb(fp, buf); }
	if(retcd < 0) {
	    csl_inq("Null enter");
		exit( retcd );
		return retcd;
	} else {
		return 0;
	}
}


/* ----------------------------------------------------------------------------
処理    コマンドライン引数の相関チェック
        　　〃　　　　　　　妥当性チェック
戻値
引数
注意
---------------------------------------------------------------------------- */
int arg_chk(struct CmdLine opt[])
{
    /* OPT_j（必須） */
    /* OPT_m（必須） */
    /* OPT_l（必須） */
    /* OPT_i（必須） */
    /* OPT_o（必須） */
    return 0;
}


/* ****************************************************************************
    初期処理−１
        開始ＭＳＧ出力をコンソール・ログファイルに出力
        引数関連処理
        ファイルオープン
**************************************************************************** */
void	ctl_init_01(int argc, char *argv[])
{
	int		i;
    /*
     *  開始ＭＳＧ
     */
    _jobname = gsPrgid;
    csl_msgs("");
    csl_msga(argv);

    /*
     *  引数の指定可不可チェック
     *  struct CmdLine options[]に待避
     */
    if(argc <= 1) {
        usage();
        uabend(NULL, "ctl_init_01", 1, "argment not specified", -1);
    }
    if((i = opt_init(argc, argv, options)) != 0) {
        sprintf(_msgbuf,"Option Error. argment %d(%s)", i, argv[i]); 
        uabend(NULL,"ctl_init_01", 2, _msgbuf, -1);
    }

    /*
     *  引数の優先処理１：-h -> ヘルプの表示して終了
     */
    if(options[OPT_h].opt_sitei == 1) {
        usage();
        csl_msge("");
        csl_inq("Null enter to exit");
        exit(0);
    }


    /*
     *  コマンドオプションの必須チェック
     */
    if((i = opt_hissu(options)) != 0) {
        sprintf(_msgbuf, "option[%c] is necessary", options[i].character); 
        uabend(NULL,"ctl_init_01", 3, _msgbuf, -1);
    }

    /*
     *  コマンドオプションのに続くargmentの必須チェック
     */
    if((i = oarg_hissu(options)) != 0) {
        sprintf(_msgbuf, "option[%c] has'nt argment", options[i].character); 
        uabend(NULL,"ctl_init_01", 4, _msgbuf, -1);
    }

    /*
     *  引数間の相関チェック
     *  引数の妥当性チェック
     */
    arg_chk(options);

    /*
     *　ファイルオープン
     */
    strcpy(jclfile.fname, options[OPT_j].argment);    
    if(f_open(&jclfile, "rt") != 0) {
        sprintf(_msgbuf,"file[%s] cannot open", jclfile.fname); 
        uabend(NULL,"ctl_init_01", 7, _msgbuf, -1);
    }

    strcpy(mapfile.fname, options[OPT_m].argment);    
    if(f_open(&mapfile, "rt") != 0) {
        sprintf(_msgbuf,"file[%s] cannot open", mapfile.fname); 
        uabend(NULL,"ctl_init_01", 8, _msgbuf, -1);
    }

    strcpy(logfile.fname, options[OPT_l].argment);  
    if(f_open(&logfile, "wt") != 0) {
        sprintf(_msgbuf, "file[%s] cannot create", logfile.fname);
        uabend(NULL,"ctl_init_01", 9, _msgbuf, -1);
    }

    strcpy(infile.fname,  options[OPT_i].argment);  /* infileとoutfileはＪＣＬファイルの内容で	*/
    strcpy(outfile.fname, options[OPT_o].argment);  /* open-modeを設定するので、オープンは後で	*/

    /*
     *  開始メッセージ（ログファイル）
     */ 
    fil_msgs(logfile.fpointer, "");
    fil_msga(logfile.fpointer, argv);
}


/* ****************************************************************************
    初期処理−２
        ＪＣＬファイルの処理
        infile.outfileのFileControlを完成
**************************************************************************** */
void	ctl_init_02()
{
	/*
	 *	ＪＣＬファイルから必要情報を取得し、チェックする
	 */
	input_config(jclfile.fname, CFG_btdtcnv,  "[btdtcnv]");
	input_config(jclfile.fname, CFG_infile,   "[infile]");
	input_config(jclfile.fname, CFG_outfile,  "[outfile]");
	input_config(jclfile.fname, CFG_mapfile,  "[mapfile]");
	input_config(jclfile.fname, CFG_codetbl,  "[codetbl]");

	/*
	 *	btdtcnv
	 */
	     if(!strcmp(gsExecute, "codeconv")) {	giExecute	=	EXE_CODECONV;	}
	else if(!strcmp(gsExecute, "unpack")) 	{	giExecute	=	EXE_UNPACK;		}
	else if(!strcmp(gsExecute, "fmtdmp")) 	{
			giExecute	=	EXE_FMTDMP;
#ifdef	DEBUG
			printf("ctl_init_02:giMaxFmtprint=%d\n", giMaxFmtprint);
#endif
																				}
	else 									{	giExecute	=	EXE_UNDEF;		}



	/*
	 * infile(gin_Code....)
	 */
	if(     strcmp(infile.s_code_han, "EBC") == 0) {	gin_CodeHan	= CODE_EBC;	}
	else if(strcmp(infile.s_code_han, "ASC") == 0) {	gin_CodeHan	= CODE_ASC;	}
	else { uabend(logfile.fpointer, "ctl_init_02", 1, "infile Hankaku code not defined", -1); }
	
	if(strcmp(infile.s_code_zen,       "JIS83" ) == 0) {	gin_CodeZen	= CODE_JIS83;	}
	else if(strcmp(infile.s_code_zen,  "SJIS"  ) == 0) {	gin_CodeZen	= CODE_SJIS;	}
	else if(strcmp(infile.s_code_zen,  "KEIS83") == 0) {	gin_CodeZen	= CODE_KEIS83;	}
	else if(strcmp(infile.s_code_zen,  "JEF83" ) == 0) {	gin_CodeZen	= CODE_JEF83;	}
	else { uabend(logfile.fpointer, "ctl_init_02", 2, "infile Hankaku code not defined", -1); }

	/*
	 * outfile(got_Code....)
	 */
	if(     strcmp(outfile.s_code_han, "EBC") == 0) {	got_CodeHan	= CODE_EBC;	}
	else if(strcmp(outfile.s_code_han, "ASC") == 0) {	got_CodeHan	= CODE_ASC;	}
	else { uabend(logfile.fpointer, "ctl_init_02", 1, "Outfile Hankaku code not defined", -1); }
	
	if(strcmp(outfile.s_code_zen,       "JIS83" ) == 0) {	got_CodeZen	= CODE_JIS83;	}
	else if(strcmp(outfile.s_code_zen,  "SJIS"  ) == 0) {	got_CodeZen	= CODE_SJIS;	}
	else if(strcmp(outfile.s_code_zen,  "KEIS83") == 0) {	got_CodeZen	= CODE_KEIS83;	}
	else if(strcmp(outfile.s_code_zen,  "JEF83" ) == 0) {	got_CodeZen	= CODE_JEF83;	}
	else { uabend(logfile.fpointer, "ctl_init_02", 2, "Outfile Hankaku code not defined", -1); }


	/*
	 * mapfile
	 */ 

	/*
	 * infile-open: オープンモードは、バイナリに固定
	 */
    if( f_open(&infile, "rb") != 0) {
        sprintf(_msgbuf,"file[%s] cannot open", infile.fname); 
        uabend(NULL,"ctl_init_02", 11, _msgbuf, -1);
    }
	/*
	 * outfile-open: オープンモードは、recfmから決める
	 *		recfm=F/FB/V/VB -> binary
	 *		recfm=T/C       -> binary (unpackＮＧのとき、出力が￥０で切れるため
	 */
	switch( toupper( *outfile.s_recfm ) ){
		case 'F':
		case 'V':
			strcpy(outfile.mode, "wb");
			break;
		case 'T':
		case 'C':
			strcpy(outfile.mode, "wb");
			break;
		default:
			uabend(logfile.fpointer, "ctl_init_02", 12, "outfile-recfm invalid", -1);
			break;
	}

    if( f_open(&outfile, "") != 0) {
        sprintf(_msgbuf,"file[%s] cannot open", outfile.fname); 
        uabend(NULL,"ctl_init_02", 13, _msgbuf, -1);
    }


    /*
     *  recfmとlreclの相関チェック
     *      recfm=F の時 infile.lrecl が必須（入力レコード長）
     *      recfm=V の時 infile.lrecl は指定不可
     */
	switch( infile.recfm ){
	case	RECFM_F:
	case	RECFM_FB:
        if(infile.lrecl > MAX_READBUF)
        {
            sprintf(_msgbuf, "infile must be less equal %d bytes", MAX_READBUF);
			fcntl_fdmp(&infile, "[infile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 20, _msgbuf, -1);
        } else if(infile.lrecl < 1) {
            sprintf(_msgbuf, "infile must be longer equal %d bytes", 1);
			fcntl_fdmp(&infile, "[infile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 21, _msgbuf, -1);
		}
		break;
	case	RECFM_V:
	case	RECFM_VB:
		if(infile.lrecl != 0)
		{
			sprintf(_msgbuf, "RECFM=V cannot specify infile.lrecl");
			fcntl_fdmp(&infile, "[infile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 22, _msgbuf, -1);
		}
		break;
    default:
			sprintf(_msgbuf, "infile RECFM is incorrect");
			fcntl_fdmp(&infile, "[infile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 23, _msgbuf, -1);
			break;
    }

    /*
     *  recfmとlreclの相関チェック
     *      recfm=F の時 outfile.lrecl が必須（入力レコード長）
     *      recfm=V の時 outfile.lrecl は指定不可
     */
	switch( outfile.recfm ){
	case	RECFM_F:
	case	RECFM_FB:
        if(outfile.lrecl > MAX_READBUF)
        {
            sprintf(_msgbuf, "outfile must be less equal %d bytes", MAX_READBUF);
			fcntl_fdmp(&outfile, "[outfile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 30, _msgbuf, -1);
        } else if(outfile.lrecl < 1) {
            sprintf(_msgbuf, "outfile must be longer equal %d bytes", 1);
			fcntl_fdmp(&outfile, "[outfile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 31, _msgbuf, -1);
		}
		break;
	case	RECFM_V:
	case	RECFM_VB:
	case	RECFM_T:
	case	RECFM_C:
		if(outfile.lrecl != 0)
		{
			sprintf(_msgbuf, "RECFM=V,T,C cannot specify outfile.lrecl");
			fcntl_fdmp(&outfile, "[outfile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 32, _msgbuf, -1);
		}
		break;
    default:
			sprintf(_msgbuf, "outfile RECFM is incorrect");
			fcntl_fdmp(&outfile, "[outfile]", logfile.fpointer);
	        uabend(NULL,"ctl_init_02", 33, _msgbuf, -1);
			break;
    }

    /*
     *  コード変換ＴＢＬの初期化
     */
	code_const_init();
	zentbl_init(codeconvctl);
	hantbl_init(codeconvctl);

}

/* ****************************************************************************
    初期処理−３
        ＭＡＰファイルの処理
**************************************************************************** */
void	ctl_init_03()
{
	char	*l;							/* mapfile入力バッファ（コメント処理後）へのポインタ		*/
	char	buf[MAX_READBUF];
	int		i,n;	
	struct	Record_ID	r;
	int		ot_start;
	char	*token;

    l = entry_gets(buf, MAX_READBUF, mapfile.fpointer);
    do {
        /*
         *  [ID]の時
         */
        if( strncmp(buf, "[ID]", 4)  == 0 )
        {
										/*
										 * 次の[まで処理する
										 */
            while( ((l = entry_gets(buf, MAX_READBUF, mapfile.fpointer)) != NULL )
                            &&
                   (*l != '[')
                 )
            {							/* 
            							 * mapfileを読んで、buf -> r(record_id)を設定する
            							 */
				if(recordid_chkset(buf, &r) != 0) {
					uabend(logfile.fpointer, "ctl_init_03", 1, "record is incorrect", -1);
				} else {
					record_id[r.rec_id].rec_id	=	r.rec_id;
					record_id[r.rec_id].start	=	r.start;
					record_id[r.rec_id].length	=	r.length;
					strcpy(record_id[r.rec_id].type, r.type);
					strcpy(record_id[r.rec_id].smin, r.smin);
					strcpy(record_id[r.rec_id].smax, r.smax);
				}
            }
        } else 
        /*
         *      [RECORD-xx]ｴﾝﾄﾘの時
         *		xxをｎに設定する
         */
        if(strncmp(buf, "[RECORD-", 8) == 0 ) {
            i = 0;
            sscanf(buf+ 8, "%2d]", &n);
            ot_start = 0;
            
            /*
             *  [RECORD-xx]が読まれていて、次"[RECORD-XX"かＥＯＦまでループ
             */
            while( ((l = entry_gets(buf, MAX_READBUF, mapfile.fpointer)) != NULL) &&
                   (strncmp(buf, "[RECORD-",8) != 0) )
            {
                /*
                 *  [RECORD-XX]の各行を読んで、item[n][],otitem[n][]に設定する
                 */
               	if( (token = strtok(buf , " ,\t\n")) != NULL ) { item[n][i].start	=	atoi(token) + giAdjust; } /* 最初は開始位置 */
               	if( (token = strtok(NULL, " ,\t\n")) != NULL ) { item[n][i].length	=	atoi(token); } /* ２番目は項目長 */
               	if( (token = strtok(NULL, " ,\t\n")) != NULL ) { strcpy(item[n][i].type, token);     } /* ３番目は属性   */
               	if( (token = strtok(NULL, " ,\t\n")) != NULL ) { strcpy(item[n][i].tag, token) ;     } /* ４番目はタグ   */

                if(!strcmp(item[n][i].type, "CH"))
                {
					item[n][i].itype = TYPE_CH;
	                otitem[n][i].start	=	ot_start;
    	            otitem[n][i].length	=	item[n][i].length;
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
				else if(!strcmp(item[n][i].type, "PD"))	
				{
					item[n][i].itype = TYPE_PD;
	                otitem[n][i].start	=	ot_start;
	                if(giExecute == EXE_UNPACK || giExecute == EXE_FMTDMP) {
    	            	otitem[n][i].length	=	item[n][i].length * 2;
    	            } else {
    	            	otitem[n][i].length	=	item[n][i].length;
					}
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else if(!strcmp(item[n][i].type, "ZD"))	
                {
					item[n][i].itype = TYPE_ZD;
	                otitem[n][i].start	=	ot_start;
	                if(giExecute == EXE_UNPACK || giExecute == EXE_FMTDMP) {
						otitem[n][i].length	=	item[n][i].length + 1;
					} else {
						otitem[n][i].length	=	item[n][i].length;
					}
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else if(!strcmp(item[n][i].type, "NC"))	
                {
					item[n][i].itype = TYPE_NC;
	                otitem[n][i].start	=	ot_start;
    	            otitem[n][i].length	=	item[n][i].length;
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else if(!strcmp(item[n][i].type, "B8"))
                {
					item[n][i].itype = TYPE_B8;
	                otitem[n][i].start	=	ot_start;
	                if(giExecute == EXE_UNPACK || giExecute == EXE_FMTDMP) {
	    	            otitem[n][i].length	=	item[n][i].length * 8;
	    	        } else {
	    	            otitem[n][i].length	=	item[n][i].length;
					}
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else if(!strcmp(item[n][i].type, "HX"))	
                {
					item[n][i].itype = TYPE_HX;
	                otitem[n][i].start	=	ot_start;
	                if(giExecute == EXE_UNPACK || giExecute == EXE_FMTDMP) {
			        	otitem[n][i].length	=	item[n][i].length * 2;
			        } else {
			        	otitem[n][i].length	=	item[n][i].length;
					}
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else if(!strcmp(item[n][i].type, "CP"))	
                {
					item[n][i].itype = TYPE_CP;
	                otitem[n][i].start	=	ot_start;
    	            otitem[n][i].length	=	item[n][i].length;
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                else
                {
					item[n][i].itype = TYPE_UNDEF;
    	            strcpy(otitem[n][i].tag, item[n][i].tag);
				}
                
                ot_start	+=	otitem[n][i].length;
                i++;
            }
        } else
        {
	        uabend(NULL,"ctl_init_03", 10, "mapfile structure invalid", -1);
        }
    } while(l != NULL);
    /* 変換項目はi個 */

#ifdef	DBG_RECIDTBL
    printf("ctl_init_03:recordid&item\n");
    for(i = 0; *record_id[i].type != '\0' ; i++) {
		printf("recid-tbl:%d=%d,%d/%s/%s/%s/\n", record_id[i].rec_id, record_id[i].start, record_id[i].length,
				record_id[i].type, record_id[i].smin, record_id[i].smax); 
		for(j = 0; *item[i][j].type != '\0' ; j++) {
			printf("item[%d:%d]=%d/%d/%s/%d/%s\n",
					i, j, item[i][j].start, item[i][j].length, item[i][j].type,item[i][j].itype, item[i][j].tag);
		}
		for(j = 0; otitem[i][j].length != 0 ; j++) {
			printf("otitem[%d:%d]=%d/%d/%s\n", i, j, otitem[i][j].start, otitem[i][j].length, otitem[i][j].tag);
		}
    }
#endif

}

/* ****************************************************************************
    終了処理
        ファイルクローズ
        ログファイルへ入出力件数の書込み
        ログファイルのクローズ
        終了ＭＳＧをコンソールへ出力
**************************************************************************** */
void	ctl_term_01()
{
    /*
     *  ファイルクローズ
     */
    f_close( &jclfile );
    f_close( &mapfile );
    f_close( &infile );
    f_close( &outfile );

    /*
     *  I/Oｶｳﾝﾄのログ書込み
     *  ログファイルクローズ
     */

    fil_msgc(logfile.fpointer, jclfile.fname, jclfile.iocnt);
    fil_msgc(logfile.fpointer, mapfile.fname, mapfile.iocnt);
    fil_msgc(logfile.fpointer, infile.fname,  infile.iocnt);
    fil_msgc(logfile.fpointer, outfile.fname, outfile.iocnt);
    fil_msge(logfile.fpointer, "");
    f_close( &logfile );

    /*
     *  Job end message 
     */
    csl_msge("");
    csl_inq("JOB has Completed. Null enter to exit");

}

/* ****************************************************************************
    レコードＩＤの設定をチェック
**************************************************************************** */
int		recordid_chkset(char *buf, struct Record_ID *recid)
{

	struct	Record_ID	r;
	char	str[MAX_READBUF];
	char	*token;


	sscanf(buf, "%d=%d,%d,%s\n", &r.rec_id, &r.start, &r.length, str);

	if(r.rec_id >= 0 && r.rec_id < MAX_RECID) {
		recid->rec_id	=	r.rec_id;
	} else {
		uabend(logfile.fpointer, "recordid_chkset", 1, "id must 00 to MAX_RECID", -1);
	}

	if(r.start + r.length > MAX_READBUF) {
		uabend(logfile.fpointer, "recordid_chkset", 2, "start + length over MAX_READBUF", -1);
	} else {
		recid->start	=	r.start;
		recid->length	=	r.length;
	}
	/*
	 *	tokenの切り出し。最初は文字列を指定、次回以降はＮＵＬＬを指定
	 *	tokenがなくなると、ＮＵＬＬを返却する。
	 */
	if( (token = strtok(str, ",\n")) != NULL )
	{
		strcpy(r.type, token);
		if( (token = strtok(NULL, ",\n")) != NULL )
		{
			strcpy(r.smin, token);
			if( (token = strtok(NULL, ",\n")) != NULL )
			{
				strcpy(r.smax, token);
			} else {
				uabend(logfile.fpointer, "recordid_chkset", 3, "record is incorrect", -1);
			} 
		} else {
			uabend(logfile.fpointer, "recordid_chkset", 4, "record is incorrect", -1);
		}
	} else {
		uabend(logfile.fpointer, "recordid_chkset", 5, "record is incorrect", -1);
	}
	
	if((strcmp(r.type, "PD") == 0) || (strcmp(r.type, "ZD") == 0) ||
	   (strcmp(r.type, "CH") == 0) || (strcmp(r.type, "NC") == 0) ||
	   (strcmp(r.type, "HX") == 0) || (strcmp(r.type, "CP") == 0) ||
	   (strcmp(r.type, "B8") == 0))
	{
		;
	} else {
		uabend(logfile.fpointer, "recordid_chkset", 6, buf, -1);
	}
	
	strcpy(recid->type, r.type);
	strcpy(recid->smin, r.smin);
	strcpy(recid->smax, r.smax);
	
	return 0;
}


/* ****************************************************************************
    入力レコードのレコードＩＤを判定する
**************************************************************************** */
int		get_recid(unsigned char *buf, struct Record_ID r[])
{
	int				value;
	int				ret;
	unsigned char	hx_min[MAX_READBUF];
	unsigned char	hx_max[MAX_READBUF];
	int				i;
	
	ret	= -1;
	for(i = 0; *r[i].type != '\0'; i++)
	{

#ifdef DBG_RECID
		printf("%d/%s:%d:%d:%s:%s\n", i, r[i].type, r[i].start, r[i].length, r[i].smin, r[i].smax);
#endif
		if(strcmp(r[i].type, "CH") == 0)
		{
			if( memcmp(r[i].smin, buf + r[i].start, r[i].length) <= 0
				&&
				memcmp(buf + r[i].start, r[i].smax, r[i].length) <= 0
			  )
			{
				ret	=	r[i].rec_id;
#ifdef DBG_RECID
				printf("rec_recid-CH:i=%d:id=%d\n", i, ret);
#endif
				break;
			}
		} else if(strcmp(r[i].type, "PD") == 0)
		{
			btpd2int(buf + r[i].start, r[i].length, &value);
			if( atoi( r[i].smin ) < value
				&&
				value <= atoi( r[i].smax ) )
			{
				ret	=	r[i].rec_id;
#ifdef DBG_RECID
				printf("rec_recid-PD:i=%d:id=%d\n", i, ret);
#endif
				break;
			}
		} else if(strcmp(r[i].type, "HX") == 0)
		{
			/*
			 *	ＭＡＰファイルを判定は、自コードとする
			 */
			btx2hex( (unsigned char *)r[i].smin, r[i].length * 2 , hx_min, r[i].length, MYCODE_HAN);
			btx2hex( (unsigned char *)r[i].smax, r[i].length * 2 , hx_max, r[i].length, MYCODE_HAN);
			if( memcmp(hx_min, buf + r[i].start, r[i].length) <= 0
				&&
				memcmp(buf + r[i].start, hx_max, r[i].length) <= 0
			  )
			{
				ret	=	r[i].rec_id;
#ifdef DBG_RECID
				printf("rec_recid-HX:i=%d:id=%d\n", i, ret);
#endif
				break;
			}
		} else
		{
#ifdef DBG_RECID
			printf("rec_recid-UNDEF:i=%d:id=%d\n", i, -1);
#endif
			return -1;
		}
	}
	return ret;
}

/* ----------------------------------------------------------------------------
概要	：コード変換出力
処理	：buf -> otbufへ出力。
		  item,otitemのフォーマット指定に従う
戻値	：otbufの有効長
引数	：
注意
---------------------------------------------------------------------------- */
int		btdtcnv_codeconv(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[])
{
	static	int	ret;
	int		i,l;
	
	l = 0;
	for(i = 0; *item[i].type != '\0'; i++)
	{
		l += otitem[i].length;
		switch ( item[i].itype ) {
		case	TYPE_CH:			/* 半角コード変換		*/
			bthancnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_PD:			/* そのままコピー		*/
			btch2hx	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_ZD:			/* 文字コード変換と、符号サイン変換	*/
			btzd2zd	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_NC:			/* 全角コード変換		*/
			btzencnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeZen);
			break;
		case	TYPE_B8:			/* そのままコピー		*/
			btch2hx	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_HX:			/* そのままコピー		*/
			btch2hx	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_CP:
			bthx2hx	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		default:
			break;
		}
	}
	return (ret = l);
}

/* ----------------------------------------------------------------------------
概要	：フォーマットダンプ出力（バッファ出力）
処理	：buf -> otbufへ出力。
		  item,otitemのフォーマット指定に従う
戻値	：otbufの有効長
引数	：
注意
---------------------------------------------------------------------------- */
int		btdtcnv_fmtdmp(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[])
{
	static	int		ret;
	int		i,l;
	
	l = 0;
	for(i = 0; *item[i].type != '\0'; i++)
	{
		l += otitem[i].length;
		switch ( item[i].itype ) {
		case	TYPE_CH:			/* 半角コード変換			*/
			bthancnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_PD:			/* マイナス編集(UNPACK)		*/
			btpd2hs (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_ZD:			/* 符号以外は半角コード変換、符号は文字コードにあわせて＋−□ */
			btzd2hs	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_NC:			/* 全角コード変換			*/
			btzencnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeZen);
			break;
		case	TYPE_B8:			/* ビットバイト変換			*/
			btbit2c (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_HX:			/* ＨＥＸダンプ				*/
			bthex2x (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		case	TYPE_CP:			/* なぜかそのままコピー		*/
			bthx2hx(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
			break;
		default:
			break;
		}
	}
	return (ret = l);
}

/* ----------------------------------------------------------------------------
概要	：フォーマットダンプ出力（ダンプ出力）
処理	：buf -> otbufへ出力。
		  item,otitemのフォーマット指定に従う
戻値	：otbufの有効長
引数	：
注意
---------------------------------------------------------------------------- */
int		fmtprint(unsigned char *otbuf, struct OtItem otitem[], FILE *fp, int ll)
{

	static	int	ret;
	int		i,l;
	char	buf[MAX_TERM];
	int		w;
	int		l_tag, l_ot;

	l = 0;
	w = 0;
	
	for(i = 0; otitem[i].length != 0; i++)
	{
		/* 
		 * tag1=(.....) tag2=(...) 
		 *
		*/
		
		l_tag	=	strlen(otitem[i].tag);
		memcpy(buf, otbuf + otitem[i].start, otitem[i].length);
		buf[ otitem[i].length ] = ' ';
		buf[ otitem[i].length + 1 ] = '\0';
		l_ot	=	strlen( buf );
		
		if(l_tag + 1 + l_ot > ll) {
			fprintf(stderr, "fmtprint:fmt-dmp item too long:%d/%s/\n", l_tag + 1 + l_ot, otitem[i].tag);
			exit(-1);
		}
		
		if(w + l_tag + 1 + l_ot > ll) {
			fprintf(fp, "\n");
			w	=	0;
		}
		
		fprintf(fp, "%s=", otitem[i].tag);
		fprintf(fp, "%s",  buf);
		w += (l_tag + 1 + l_ot);
	}
	fprintf(fp, "\n");
	return (ret = w);
}

/* ----------------------------------------------------------------------------
概要	：アンパック
処理	：buf -> otbufへ出力。
		  item,otitemのフォーマット指定に従う
戻値	：otbufの有効長
引数	：
注意
---------------------------------------------------------------------------- */
int		btdtcnv_unpack(unsigned char *buf, struct Item item[], unsigned char *otbuf, struct OtItem otitem[], char delimiter)
{
	static	int		ret;
	int		i,l;
	int		st_hosei;

	if(delimiter == '#' || delimiter == '\0') 		/* セパレータなし	*/
	{
		l = 0;
		for(i = 0; *item[i].type != '\0'; i++)
		{
			l += otitem[i].length;
			switch ( item[i].itype ) {
			case	TYPE_CH:			/* 半角コード変換			*/
				bthancnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			case	TYPE_PD:			/* マイナス編集				*/
				btpd2hs (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			case	TYPE_ZD:			/* 符号以外は半角コード変換、符号は文字コードにあわせて＋−□ */
				btzd2hs	(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			case	TYPE_NC:			/* 全角コード変換			*/
				btzencnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeZen);
				break;
			case	TYPE_B8:			/* ビットバイト変換			*/
				btbit2c (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			case	TYPE_HX:			/* ＨＥＸダンプ				*/
				bthex2x (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			case	TYPE_CP:			/* なぜかそのままコピー		*/
				bthx2hx (buf + item[i].start, item[i].length, otbuf + otitem[i].start, otitem[i].length, got_CodeHan);
				break;
			default:
				break;
			}
		}
		return (ret = l);
	}
	else												/* セパレータあり	*/
	{
		l = 0;
		st_hosei = 0;
		for(i = 0; *item[i].type != '\0'; i++)
		{
			l += (otitem[i].length + 1);
			switch ( item[i].itype ) {
			case	TYPE_CH:			/* 半角コード変換		*/
				bthancnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_PD:			/* マイナス編集			*/
				btpd2hs (buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_ZD:			/* 符号以外は半角コード変換、符号は文字コードにあわせて＋−□ */
				btzd2hs	(buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_NC:			/* 全角コード変換		*/
				btzencnv(buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeZen);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_B8:			/* ビットバイト変換		*/
				btbit2c (buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_HX:			/* ＨＥＸダンプ			*/
				bthex2x (buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			case	TYPE_CP:			/* なぜかそのままコピー	*/
				bthx2hx (buf + item[i].start, item[i].length, otbuf + otitem[i].start + st_hosei, otitem[i].length, got_CodeHan);
				otbuf[ otitem[i].start + st_hosei + otitem[i].length ] = delimiter;
				break;
			default:
				break;
			}
			
			st_hosei += 1;
		}
		return (ret = l - 1);			/* 最後のデリミタは出力しない	*/
	}
}

/* ----------------------------------------------------------------------------
処理	:fcntlのメンバをダンプ
戻値	:なし
引数	:
注意	:
---------------------------------------------------------------------------- */
int		fcntl_fdmp(struct FileControl *fcntl, char *msg, FILE *fp)
{
	char	buf[128];

													fil_msg(fp, msg);
	sprintf(buf, "  fname=%s", fcntl->fname); 		fil_msg(fp, buf);
	sprintf(buf, "  iocnt=%ld", fcntl->iocnt);		fil_msg(fp, buf);
	sprintf(buf, "  recfm=%d", fcntl->recfm);		fil_msg(fp, buf);
	sprintf(buf, "  lrecl=%d", fcntl->lrecl);     	fil_msg(fp, buf);
	sprintf(buf, "  blksz=%d", fcntl->blksize);   	fil_msg(fp, buf);
	sprintf(buf, "  eof  =%d", fcntl->eof);       	fil_msg(fp, buf);
	sprintf(buf, "  bklp =%d", fcntl->blkp);      	fil_msg(fp, buf);
	sprintf(buf, "  cr   =%d", fcntl->cr);        	fil_msg(fp, buf);
	sprintf(buf, "  delim=%c", fcntl->delimiter); 	fil_msg(fp, buf);
	sprintf(buf, "  recfm=%s", fcntl->s_recfm);   	fil_msg(fp, buf);
	sprintf(buf, "  mode =%s", fcntl->mode);      	fil_msg(fp, buf);
	sprintf(buf, "  han  =%s", fcntl->s_code_han);	fil_msg(fp, buf);
	sprintf(buf, "  zen  =%s", fcntl->s_code_zen);	fil_msg(fp, buf);
	sprintf(buf, "  vbtyp=%s", fcntl->s_vbtype);	fil_msg(fp, buf);
	
	return 0;

}


/* ----------------------------------------------------------------------------
	コード変換ＣＴＬのダンプ
---------------------------------------------------------------------------- */
int		codeconvctl_fdmp(struct CodeConvCTL ctl, char *msg, FILE *fp)
{
	char	buf[128];

	sprintf(buf, "%s", msg);							fil_msg(fp, buf);
	sprintf(buf, "  han_path =%s", ctl.tbl_han_path);		fil_msg(fp, buf);
	sprintf(buf, "  zen_path =%s", ctl.tbl_zen_path);		fil_msg(fp, buf);
	sprintf(buf, "  abend_han=%d", ctl.abend_han);		fil_msg(fp, buf);
	sprintf(buf, "  abend_zen=%d", ctl.abend_zen);		fil_msg(fp, buf);
	sprintf(buf, "  rep_han  =%c:%x", 
		(unsigned char)ctl.replace_han, (unsigned char)ctl.replace_han);
														fil_msg(fp, buf);
	sprintf(buf, "  rep_zen  =:%x",   ctl.replace_zen);	fil_msg(fp, buf);
	return 0;
}
