/*
 ***********************************************************************
 *	$Id: btdtcnv.h,v 1.4 2006/07/17 07:25:29 hiroshi Exp $
 ***********************************************************************
 *	$Log: btdtcnv.h,v $
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
 * file	：btdtcnv.h
 * 機能	：btdtcnv.cのヘッダファイル。マクロなど
 * 履歴	2005/09/13	COBOLから移植開始
 */

#ifndef		_INC_BTDTCNV
#define		_INC_BTDTCNV

/* ----------------------------------------------------------------------------
	ＩＮＣＬＵＤＥファイル
---------------------------------------------------------------------------- */
#include	"cmdline.h"
#include	"filecntl.h"
#include	"cfgopts.h"
#include	"codecnv2.h"
#include	<stdio.h>
#include	<string.h>

/* ----------------------------------------------------------------------------
	ＤＥＦＩＮＥ＆ＭＡＣＲＯ
---------------------------------------------------------------------------- */
#define     MAX_RECID       50      /* mapFileで指定するレコードＩＤの最大数            */
#define     MAX_ITEM        2000    /* mapFileで指定するレコードＩＤ別項目定義の最大数  */
#define	    MAX_TERM	    1024    /* レコードの１項目の最大項目長						*/

#define     MAX_READBUF     32768    /* infileの最大レコード長               */
#define     MAX_WRITEBUF    32768    /* outfileの最大レコード長              */


#define     OPT_j           0       /* sysin-file   */
#define     OPT_m           1       /* map-file     */
#define     OPT_l           2       /* log-file     */
#define     OPT_i           3       /* input-file   */
#define     OPT_o           4       /* output-file  */
#define     OPT_h           5       /* help         */
#define     OPT_d           6       /* dump         */

#define		EXE_UNDEF		0		/* execute=NOTDEFINED */
#define		EXE_CODECONV	1		/* execute=codeconv	*/
#define		EXE_UNPACK		2		/* execute=unpack	*/
#define		EXE_FMTDMP		3		/* execute=fmtdmp	*/

#define		TYPE_UNDEF		0
#define		TYPE_CH			1
#define		TYPE_PD			2
#define		TYPE_ZD			3
#define		TYPE_NC			4
#define		TYPE_B8			5
#define		TYPE_HX			6
#define		TYPE_CP			7


/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */
struct	Record_ID	{
	int		rec_id;
	int		start;
	int		length;
	char	type[3];
	char	smin[MAX_TERM];
	char	smax[MAX_TERM];
} ;

struct	Item {					/* mapfileの[record] */
	int		start;
	int		length;
	char	type[3];
	int		itype;
	char	tag[128];
} ;

struct	OtItem {				/* mapfileの[record] */
	int		start;
	int		length;
	char	tag[128];
} ;



/* ----------------------------------------------------------------------------
	関数プロトタイプ宣言
---------------------------------------------------------------------------- */


#ifdef  MAIN
/* ----------------------------------------------------------------------------
	外部変数の定義
---------------------------------------------------------------------------- */
#define     MAX_USAGECOLUM  80      /* Usage出力ﾊﾞｯﾌｧの最大長 */

    static char gsPrgid[] = "bdtcnv"; 
    static char gsVersion[][MAX_USAGECOLUM] = {
  /* 123456789*123456789*123456789*123456789*123456789*123456789*123456789*123456789* */
    "",
    "Code converter Btdtcnv Ver.0.0.1 Copyright (c) 2005, Hiroshi Harasaki",
    "usage1:dtcnvexe -j(jcl) -m(recordmap) -l(log) -i(in) -o(out)",
    "",
    "属性変換対応表",
    "  入力属性 | 変換パターン | 出力長",
    "      CH   | 半角変換     | n     ",
    "      PD   | -99999..9    | 2*n   ",
    "      ZD   | 半角変換     | n or n+1",
    "      NC   | 全角変換     | n     ",
    "      CP   | 無変換       | n     ",
    "      HX   | HEX-DMP      | 2*n   ",
    "      B8   | キャラビット | 8*n   ",
    "",
    "usage2:dtcnvexe -d[HANTBL|ZENTBL]   変換ﾃｰﾌﾞﾙのﾀﾞﾝﾌﾟ",
    "/"
    } ;


    /*
     *  引数のﾁｪｯｸ判定用内部テーブル
     *  このﾃｰﾌﾞﾙｻｰﾁの終了判定のため、最後の要素は{(char)NULL...}で終る
     *    <1>引数文字
     *    <2>引数へのポインタ
     *    <3>このオプションが必須か
     *    <4>オプションに続くargmentが必須が
     *    <5>オプションが指定されたかFLG
     *    <6>オプションに続くargmentがされたがFLG
     */ 
    static  struct CmdLine      options[] = {
/*        <1>  <2>  <3><4><5><6>				*/
        { 'j', NULL, 1, 1, 0 ,0 },
        { 'm', NULL, 1, 1, 0, 0 },
        { 'l', NULL, 1, 1, 0, 0 },
        { 'i', NULL, 1, 1, 0, 0 },
        { 'o', NULL, 1, 1, 0, 0 },
        { 'h', NULL, 0, 0, 0, 0 },
        { 'd', NULL, 0, 0, 0, 0 },
        { (char)NULL,NULL, 0, 0, 0, 0 }
    } ;

    static struct FileControl   infile;
    static struct FileControl   outfile;
    static struct FileControl   jclfile;
    static struct FileControl   mapfile;
    static struct FileControl   logfile;
    
    static	int					giAdjust;
    static	char				gsExecute[16];
    static	int					giExecute;
    static	int					giMaxFmtprint;
	static	int					giMaxincount;
    static struct CodeConvCTL	codeconvctl;

struct Config_Tag	CFG_btdtcnv[] = {
   { "execute", 	String_Tag, gsExecute	},
   { "maxfmtprint",	Word_Tag,	&giMaxFmtprint },
   { "maxicount",   Word_Tag,	&giMaxincount  },
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_infile[] = {
   { "recfm",		String_Tag,	&infile.s_recfm		},
   { "lrecl",		Word_Tag,	&infile.lrecl		},
   { "code_han", 	String_Tag, &infile.s_code_han	},
   { "code_zen", 	String_Tag, &infile.s_code_zen	},
   { "vbtype", 		String_Tag, &infile.s_vbtype		},
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_outfile[] = {
   { "recfm",		String_Tag,	&outfile.s_recfm	},
   { "lrecl",		Word_Tag,	&outfile.lrecl		},
   { "delimiter",	Char_Tag,	&outfile.delimiter	},
   { "cr", 			Word_Tag,	&outfile.cr 		},
   { "code_han", 	String_Tag, &outfile.s_code_han },
   { "code_zen", 	String_Tag, &outfile.s_code_zen },
   { "vbtype", 		String_Tag, &outfile.s_vbtype		},
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_mapfile[] = {
	/* adjust DWord_Tag -> Word_Tag */
	{ "adjust",		Word_Tag,	&giAdjust			},
	{ NULL,			Error_Tag,	NULL				}
} ;

struct Config_Tag	CFG_codetbl[] = {
	/* replace_zen DHexWord_Tag -> HexWord_Tag */
	{ "tbl_han",	String_Tag,	&codeconvctl.tbl_han_path	},
	{ "tbl_zen",	String_Tag,	&codeconvctl.tbl_zen_path	},
	{ "abend_han",	Word_Tag,	&codeconvctl.abend_han		},
	{ "abend_zen",	Word_Tag,	&codeconvctl.abend_zen		},
	{ "replace_han",	HexWord_Tag, &codeconvctl.replace_han	},
	{ "replace_zen",	HexWord_Tag, &codeconvctl.replace_zen	},
	{ NULL,			Error_Tag,	NULL	}
} ;

static struct Record_ID		record_id[MAX_RECID];
static struct Item			item[MAX_RECID][MAX_ITEM];
static struct OtItem		otitem[MAX_RECID][MAX_ITEM];


/* ----------------------------------------------------------------------------
	関数定義実態
---------------------------------------------------------------------------- */


#else
/* ----------------------------------------------------------------------------
	外部変数(static)の宣言
---------------------------------------------------------------------------- */
extern 	static	char	gsPrgid[]; 
extern 	static	char	*gsVersion[];
extern	static struct CmdLine       options[];

extern	static struct FileControl   infile;
extern	static struct FileControl   outfile;
extern	static struct FileControl   jclfile;
extern	static struct FileControl   mapfile;
extern	static struct FileControl   logfile;

extern	static	int					giAdjust;
extern  static	char				gsExecute[];
extern  static	int					giExecute;
extern  static	int					giMaxFmtprint;
extern	static	int					giMaxincount;
extern	static struct CodeConvCTL	codeconvctl;


extern	static struct Record_ID		record_id[MAX_RECID];
extern	static struct Item			item[MAX_RECID][MAX_ITEM];
extern	static struct OtItem		otitem[MAX_RECID][MAX_ITEM];

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_FILECNTL */

