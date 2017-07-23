/*
 ***********************************************************************
 *	$Id: filecntl.h,v 1.4 2006/07/17 07:25:30 hiroshi Exp $
 ***********************************************************************
 *	$Log: filecntl.h,v $
 *	Revision 1.4  2006/07/17 07:25:30  hiroshi
 *	*** empty log message ***
 *	
 *	Revision 1.3  2006/06/25 16:04:11  hiroshi
 *	可変長ファイルはホスト／ネットＣＯＢの両方を対応。
 *	
 *	Revision 1.2  2006/06/25 13:25:18  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 *
 * ﾌｧｲﾙ	ファイルを扱うためのヘッダーファイル
 * 履歴	2005/09/18	FileControl2の追加
		2005/10/10	Solaris Largefile 対応。fopen => fopen64
 */

#ifndef		_INC_FILECNTL
#define		_INC_FILECNTL

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#ifdef		SOLARIS
#define		_fopen(fp, mode)		fopen64(fp, mode)
#else
#define		_fopen(fp, mode)		fopen(fp, mode)
#endif



#define		MAXL_FILENAME	256		/* ファイル名バッファの最大長			*/
#define		MAXL_ENTRY		12		/* iniﾌｧｲﾙで[ｴﾝﾄﾘ名]の最大長			*/
#define		MAXL_KEY		12		/* 　〃       keyの最大長				*/
#define		MAXL_VALUE		128		/* 　〃       valueの最大長				*/
#define		MAXL_ENTRYBUF	256		/* 　〃     行バッファの最大長			*/

#define		FREAD_EOF		(-1)	/* fread()でのEOFはfeof()で判定するが	*/
									/* EOFの時はゼロ以外，notEOFは０		*/

#define		FCNTL_EOF		(-1)	/* FileControl.eofはeof(-1)とする		*/

#define		RDW_LENGTH		4		/* 可変長形式のレコード長部の長さ		*/
#define		BDW_LENGTH		4		/* ブロック化形式のブロック長部の長さ	*/

#define		RECFM_UNDEF		0
#define		RECFM_F			1		/* FileControl.recfm=F					*/
#define		RECFM_FB		2		/* FileControl.recfm=FB					*/
#define		RECFM_V			3		/* FileControl.recfm=V					*/
#define		RECFM_VB		4		/* FileControl.recfm=VB					*/
#define		RECFM_T			5		/* FileControl.recfm=F		固定長+\n	*/
#define		RECFM_C			6		/* FileControl.recfm=C		CSVﾌｧｲﾙ		*/

#define		VB_DEFAULT		2
#define		VB_HOST_BIG		1		/* BigEndian LLはデータ＋ＲＤＷ			*/
#define		VB_NETC_LTL		2		/* NetCOBOLのLittleEndian LLはデータ長、LL+データ+LL	*/

/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */
struct FileControl1					/* ヴァージョンアップ前	*/
{
	char	fname[MAXL_FILENAME];	/* ﾌｧｲﾙ名			*/
	FILE	*fpointer;				/* ﾌｧｲﾙへのﾎﾟｲﾝﾀ	*/
	long	iocnt;					/* IOｶｳﾝﾀ			*/
	int		recfm;					/* Recfm			*/
	int		lrecl;					/* Lrecl			*/
	int		blksize;				/* Blksize			*/
	int		eof;					/* if_EOF			*/
	int		blkp;					/* 読込済ﾌﾞﾛｯｸ数	*/
} ;

struct FileControl
{
	char	fname[MAXL_FILENAME];	/* ﾌｧｲﾙ名			*/
	FILE	*fpointer;				/* ﾌｧｲﾙへのﾎﾟｲﾝﾀ	*/
	long	iocnt;					/* IOｶｳﾝﾀ			*/
	int		recfm;					/* Recfm			*/
	int		lrecl;					/* Lrecl			*/
	int		blksize;				/* Blksize			*/
	int		eof;					/* if_EOF			*/
	int		blkp;					/* 読込済ﾌﾞﾛｯｸ数	*/
	int		vbtype;					/* 可変長タイプ		*/
/* */
	int		cr;						/* 出力時\nを付けるか	*/
	char	delimiter;				/* CSV出力のデリミタ	*/
	char	s_recfm[3];				/* Recfm			*/
	char	s_code_han[8];			/* このﾌｧｲﾙの半角文字コード */
	char	s_code_zen[8];			/* このﾌｧｲﾙの全角文字コード */
	char	mode[3];				/* ｱｸｾｽﾓｰﾄﾞ wb rb	*/
	char	s_vbtype[8];			/* 可変長タイプ		*/
} ;

/* *************************************************************
ＩＮＩファイル構造体
************************************************************** */
/*
 *	[section]
 *	key=value ... entry
 */

struct Entry {
	char	key[MAXL_KEY];			/* sysinで指定するkey */
	char	value[MAXL_VALUE];		/* key=**の引数       */
	int		hissu;					/* 必須(1)か任意か    */
	int		sitei;					/* keyが指定(1)か否か */
} ;

/* ----------------------------------------------------------------------------
	関数プロトタイプ宣言
---------------------------------------------------------------------------- */
int		inifile_init(struct FileControl *inifile, struct Entry *cktbl);
int		entry_chkset(char *cp, struct Entry *checktbl);
char	*entry_gets(char *buf, int length, FILE *fp);

int		f_open(struct FileControl *fcntl, char *mode);
int		f_close(struct FileControl *fcntl);

int		f_read( unsigned char *buf, int length, struct FileControl *fcntl);
int		f_readF(unsigned char *buf, struct FileControl *fcntl);
int		f_readV(unsigned char *buf, struct FileControl *fcntl);
int		f_readT(unsigned char *buf, int maxl, struct FileControl *fcntl);
int		f_readBLK(struct FileControl *fcntl);
int		f_readANY(unsigned char *buf, struct FileControl *fcntl);

int		f_write( unsigned char *buf, int length, struct FileControl *fcntl);
int		f_writeF(unsigned char *buf, int length, struct FileControl *fcntl, unsigned char padding);
int		f_writeV(unsigned char *buf, int length, struct FileControl *fcntl);
int		f_writeT(unsigned char *buf, struct FileControl *fcntl);
int		f_writeANY(unsigned char *buf, int length, struct FileControl *fcntl, unsigned char padding);

int		fcntl_dmp(struct FileControl *fcntl, char *msg, FILE *fp);

/*
 *	f_open/read/write/close でのfcntl設定値サマリ
 *
 *				f_open		f_close
 *	---------------------------------
 *	fname		i			void
 *	fpointer	o			i
 *	iocnt		o(0)		void
 *	recfm		void		void
 *	lrecl		void		void
 *	blksize		o(0)		o(0)
 *	eof			o(0)		o(-1)
 *	blkp		o(0)		0(0)
 *
 *				f_read		freadF		f_readV		f_readBLK	f_readVB
 *	--------------------------------------------------------------------
 *	fname		void		void		void		void		void
 *	fpointer	i			i			i			i			i
 *	iocnt		o(++)		o(++)		o(++)		void		o(++)
 *	recfm		void		void		void		void		void
 *	lrecl		void		i			o			void		void
 *	blksize		void		void		void		o			o
 *	eof			o			o			o			o			o
 *	blkp		void		void		void		void		o(++)
 *
 *				f_write		f_writeF	f_writeV	fwrite_T
 *	----------------------------------------------------------
 *	fname		void		void		void		void
 *	fpointer	i			i			i			i
 *	iocnt		o(++)		o(++)		o(++)		o(++)
 *	recfm		void		void		void		void
 *	lrecl		void		i			o			void
 *	blksize		void		void		void		void
 *	eof			void		void		void		void
 */

#ifdef		MAIN


/* ----------------------------------------------------------------------------
	外部(static)変数の定義
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
	関数の定義
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、
		　ファイルをオープンし、IOCNTを初期化する。
戻値	：0(正常）/以外エラー
引数	：
	io	  *fcntl	ファイルコントロール構造体
	o	  ->fpointer	ポインタを設定
	o	  ->iocnt		０で初期化
	o	  ->eof			０で初期化
	o	  ->blkp		０で初期化
	i	  *mode		fopen()のモード
注意
---------------------------------------------------------------------------- */
int		f_open(struct FileControl *fcntl, char *mode)
{
	FILE	*fp;
	
	fcntl->iocnt = 0;
	fcntl->eof   = 0;
	fcntl->blkp  = 0;
	fcntl->blksize = 0;
	if(strlen(fcntl->fname) == 0) {
		return -1;
	}

/* open-mode */
	if( strlen(mode) != 0 ){
		if((fp = _fopen(fcntl->fname, mode)) != NULL) {
			fcntl->fpointer = fp;
			strcpy(fcntl->mode, mode);
		} else {
			return -1;
		}
	} else {
		if((fp = _fopen(fcntl->fname, fcntl->mode)) != NULL) {
			fcntl->fpointer = fp;
		} else {
			return -1;
		}
	}
	
/* recfm */
	if(strlen(fcntl->s_recfm) != 0) {
		     if(!strcmp(fcntl->s_recfm, "FB"))	{ fcntl->recfm	=	RECFM_FB;	}
		else if(!strcmp(fcntl->s_recfm, "F"))	{ fcntl->recfm	=	RECFM_F;	}
		else if(!strcmp(fcntl->s_recfm, "VB" ))	{ fcntl->recfm	=	RECFM_VB;	}
		else if(!strcmp(fcntl->s_recfm, "V"))	{ fcntl->recfm	=	RECFM_V;	}
		else if(!strcmp(fcntl->s_recfm, "T" ))	{ fcntl->recfm	=	RECFM_T;	}
		else if(!strcmp(fcntl->s_recfm, "C" ))	{ fcntl->recfm	=	RECFM_C;	}
		else 									{ fcntl->recfm	=	-1;			}
	} else {
		switch( fcntl->recfm ) {
		case RECFM_F:	{		strcpy(fcntl->s_recfm, "F"); 		break;	}
		case RECFM_FB:	{		strcpy(fcntl->s_recfm, "FB");		break;	}
		case RECFM_V:	{		strcpy(fcntl->s_recfm, "V");		break;	}
		case RECFM_VB:	{		strcpy(fcntl->s_recfm, "VB");		break;	}
		case RECFM_T:	{		strcpy(fcntl->s_recfm, "T");		break;	}
		case RECFM_C:	{		strcpy(fcntl->s_recfm, "C");		break;	}
		default:		{											break;	}
		}
	}
	
/* vbtype RECFM=Vx のとき */
	if( fcntl->recfm == RECFM_VB || fcntl->recfm == RECFM_V )
	{
		if(strlen(fcntl->s_vbtype) != 0) {
				if(!strcmp(fcntl->s_vbtype, "host"))	{ fcntl->vbtype	=	VB_HOST_BIG;	}
			else if(!strcmp(fcntl->s_vbtype, "netcl"))	{ fcntl->vbtype	=	VB_NETC_LTL;	}
			else										{ fcntl->vbtype	=	VB_DEFAULT;				}
		} else {
			switch( fcntl->vbtype ) {
			case VB_HOST_BIG:	{	strcpy(fcntl->s_vbtype, "host");	break;	}
			case VB_NETC_LTL:	{	strcpy(fcntl->s_vbtype, "netcl");	break;	}
			default:			{	fcntl->vbtype = VB_DEFAULT; 		break;	}
			}
		}
	}
	return 0;
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、
		　ファイルをクローズし、eofをオンにする
戻値	：0(正常）/以外エラー
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		->eof		FCNTL_EOFを無条件設
	o		->blkp		０で初期化
注意
---------------------------------------------------------------------------- */
int		f_close(struct FileControl *fcntl)
{
	
	fcntl->eof   = FCNTL_EOF;
	fcntl->blkp  = 0;
	fcntl->blksize = 0;
	if(fclose(fcntl->fpointer) == 0) {
		return 0;								/* close成功	*/
	} else {
		return -1;								/* 失敗			*/
	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		　IOCNT++する．
戻値	：読み込んだバイト数．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		->iocnt		インクリメント
	o		->eof		eofの時，FCNTL_EOFを設定
	i		length	writeするbufの長さ
注意
	bufは\x00で終端されていなくてもかまわない．
---------------------------------------------------------------------------- */
int		f_read(unsigned char *buf, int length, struct FileControl *fcntl)
{
	static	int	ret;
	
	fread(buf, length, 1, fcntl->fpointer);
	if( feof(fcntl->fpointer) ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		fcntl->iocnt++;
		return (ret = length);
	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		  fcntl->recfmにより、f_readxxを呼ぶ
戻値	：f_readxxの返却値．ＥＯＦはFREAD_EOF(-1)
引数	：
注意
---------------------------------------------------------------------------- */
int		f_readANY(unsigned char *buf, struct FileControl *fcntl)
{
	switch( fcntl->recfm ){
		case RECFM_F:
		case RECFM_FB:
			return f_readF(buf, fcntl);
		case RECFM_V:
		case RECFM_VB:
			return f_readV(buf, fcntl);
		default:
			break;
	}
	return -1;
}


/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		  fcntl->lreclのレコード長分を固定長形式の読込をする．
		　IOCNT++する．
戻値	：読み込んだバイト数．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		ｰ>iocnt		インクリメント
	i		->lrecl		readする長さ
	o		->eof		eofの時，FCNTL_EOFを設定
注意
---------------------------------------------------------------------------- */
int		f_readF(unsigned char *buf, struct FileControl *fcntl)
{
	
	fread(buf, fcntl->lrecl, 1, fcntl->fpointer);
	if( feof(fcntl->fpointer) ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		fcntl->iocnt++;
		return fcntl->lrecl;
	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		  可変長形式の読込をする．
		　IOCNT++する．
		  fcntl->lreclに可変長のレコード長を設定する．
戻値	：読み込んだバイト数．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		->iocnt		インクリメント
	o		->lrecl		読み込んだ長さ（可変長形式）
	o		->eof		eofの時，FCNTL_EOFを設定
注意
---------------------------------------------------------------------------- */
int		f_readV(unsigned char *buf, struct FileControl *fcntl)
{
	unsigned char	rdw[RDW_LENGTH];
	int				lrecl;
	union	{
		unsigned char	c[2];
		unsigned short	x;
	} ll;
	
	/* RDW部を読んでレコード長を算出 */
	switch( fcntl->vbtype ) {
	case VB_HOST_BIG:
		fread(rdw, RDW_LENGTH, 1, fcntl->fpointer);
		ll.c[0] = rdw[1];						/* エンディアンをひっくりかえす */
		ll.c[1] = rdw[0];

												/* EOFならすぐ戻る。 */
		if(feof(fcntl->fpointer)) { 
			fcntl->eof = FCNTL_EOF;
			return FREAD_EOF;
		}
		lrecl = ll.x - RDW_LENGTH;				/* データ長は４バイト小 */
		fread(buf, lrecl, 1, fcntl->fpointer);	/* 実データを読む		*/
		fcntl->lrecl = ll.x;					/* データ長はそのまま	*/
		fcntl->iocnt++;
		return fcntl->lrecl;
		break;
	case VB_NETC_LTL:
		fread(rdw, RDW_LENGTH, 1, fcntl->fpointer);
		ll.c[0] = rdw[0];						/* エンディアンはそのまま */
		ll.c[1] = rdw[1];
	
		if(feof(fcntl->fpointer)) { 
			fcntl->eof = FCNTL_EOF;
			return FREAD_EOF;
		}
		lrecl = ll.x;							/* データ長はそのまま	*/
		fread(buf, lrecl, 1, fcntl->fpointer);	/* 実データを読む		*/

		fread(rdw, RDW_LENGTH, 1, fcntl->fpointer);	/* レコード後のＬＬ	*/
		ll.c[0] = rdw[0];						/* エンディアンはそのまま */
		ll.c[1] = rdw[1];

		if(lrecl != ll.x) {
			fprintf(stderr, "### ERR! f_readV lrecl confrict ###\n");
			exit(-1);
		}			

		fcntl->lrecl = ll.x;					/* データ長はそのまま	*/
		fcntl->iocnt++;
		return fcntl->lrecl;
	default:
		fprintf(stderr, "### ERR! f_readV vbtype not defined ###\n");
		exit(-1);
		
	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		  fgetsを使って￥ｎまで読込をする．
		　IOCNT++する．
		  fcntl->lreclに￥ｎを含むレコード長を設定する．
戻値	：読み込んだバイト数．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		->iocnt		インクリメント
	o		->lrecl		読み込んだ長さ（￥ｎを含む長さ）
	o		->eof		eofの時，FCNTL_EOFを設定
注意
	ﾌｧｲﾙがﾃｷｽﾄﾓｰﾄﾞｵｰﾌﾟﾝの時は，\nは0x0aに変換されている．
	      ﾊﾞｲﾅﾘﾓｰﾄﾞｵｰﾌﾟﾝ       0x0d,0x0aに２バイトに変換されている．
	ｵｰﾌﾟﾝﾓｰﾄﾞで返却値（読み込んだ長さ）が異なる．
---------------------------------------------------------------------------- */
int		f_readT(unsigned char *buf, int maxl, struct FileControl *fcntl)
{
	static	int	ret;
	int			lrecl;
	
	if( fgets((char *)buf, maxl, fcntl->fpointer) == NULL ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		fcntl->iocnt++;
		lrecl = strlen( (char *)buf );
		return (ret = lrecl);
	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、ブロックをfreadする
		  ブロック化形式の読込をする．
		  fcntl->blksizeにブロック長を設定する．
戻値	：ブロック長．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl		ファイルコントロール構造体
	o		->blksize		ブロック長（可変長形式）
	o		 ->eof			eofの時，FCNTL_EOFを設定
注意
	iocntはインクリメントしない
---------------------------------------------------------------------------- */
int		f_readBLK(struct FileControl *fcntl)
{
	unsigned char		bdw[BDW_LENGTH];
	union	{
		unsigned char	c[2];
		unsigned short	x;
	} ll;

	/*	BLK部を読んでブロック長を算出 */
	switch( fcntl->vbtype ) {
	case VB_HOST_BIG:
		fread(bdw, BDW_LENGTH, 1, fcntl->fpointer);
		ll.c[0] = bdw[1];
		ll.c[1] = bdw[0];
	
		if( feof(fcntl->fpointer) ) {
			fcntl->eof = FCNTL_EOF;
			return FREAD_EOF;
		}
		else {
			fcntl->blksize = ll.x;
			return fcntl->blksize;
		}
		break;
	case VB_NETC_LTL:
		return -1;
		break;
	default:
		return -1;
		break;
	}		
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、可変長レコードをfreadする
		  fcntl->blksizeにブロック長を設定する．
		  読込済ブロックがブロック長に等しい時は、ブロック長を読んでから
		  レコードを読む
戻値	：ブロック長．ＥＯＦはFREAD_EOF(-1)
引数	：
	io		*fcntl		ファイルコントロール構造体
	o		->blksize		ブロック長（可変長形式）
	o		->eof			eofの時，FCNTL_EOFを設定
	o		->blkp			読込済のブロックないバイト数（ポインタ）
注意
	f_readVでiocntを
		
---------------------------------------------------------------------------- */
int		f_readVB(unsigned char *buf, struct FileControl *fcntl)
{
	static	int		ret;
	int		lrecl;
	
	/*
	 *	初期読込(blksize==blkp==0) or BLK先頭(blksize==blkp)
	 *	BLK部を読んでブロック長を算出
	 */
	if(fcntl->blksize == fcntl->blkp) {
		if( f_readBLK(fcntl) == FREAD_EOF) {
			return FREAD_EOF;
		}
		else {
			fcntl->blkp = BDW_LENGTH;
		}
	}
	
	if( (lrecl = f_readV(buf, fcntl)) == FREAD_EOF ) {
		fcntl->blkp += lrecl;
		return FREAD_EOF;
	}
	else {
		fcntl->blkp += lrecl;
		return (ret = lrecl);
	}
}


/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufをwriteする
		　IOCNT++する．
戻値	：writeしたバイト数
引数	：
	io		*fcntl		ファイルコントロール構造体
	o		 ->iocnt		インクリメント
	i		length		writeするbufの長さ
注意
	bufは0x00で終端されていなくてもかまわない．
---------------------------------------------------------------------------- */
int		f_write(unsigned char *buf, int length, struct FileControl *fcntl)
{
	static int	ret;
	
	fcntl->iocnt++;
	fwrite(buf, length, 1, fcntl->fpointer);
	return (ret = length);
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufへfreadする
		  fcntl->recfmにより、f_writexxを呼ぶ
戻値	：f_writexxの返却値．ＥＯＦはFREAD_EOF(-1)
引数	：
注意
---------------------------------------------------------------------------- */
int		f_writeANY(unsigned char *buf, int length, struct FileControl *fcntl, unsigned char padding)
{
	static	int	ret;
	int	l;
	switch( fcntl->recfm ){
		case RECFM_F:
		case RECFM_FB:
			l = f_writeF(buf, length, fcntl, padding);
			switch(fcntl->cr)
			{
			case 0:
				return (ret = l);
			case 1:
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+1);
			case 2:
				fputc(0x0d, fcntl->fpointer);
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+2);
			default:
				return (ret = l);
			}
		case RECFM_V:
		case RECFM_VB:
			l = f_writeV(buf, length, fcntl);
			switch(fcntl->cr)
			{
			case 0:
				return (ret = l);
			case 1:
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+1);
			case 2:
				fputc(0x0d, fcntl->fpointer);
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+2);
			default:
				return (ret = l);
			}
		case RECFM_T:
		case RECFM_C:
			l = f_write(buf, length, fcntl);
			switch(fcntl->cr)
			{
			case 0:
				return (ret = l);
			case 1:
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+1);
			case 2:
				fputc(0x0d, fcntl->fpointer);
				fputc(0x0a, fcntl->fpointer);
				return (ret = l+2);
			default:
				return (ret = l);
			}
		default:
			return -1;
	}
}
/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufをwriteする
		　IOCNT++する．
		  bufの長さをlengthで指定し、fcntl->lreclに不足する部分は
		  paddingで埋める.余るぶんは切り捨てる
戻値	：writeしたバイト数
引数	：
	io		*fcntl		ファイルコントロール構造体
	o		->iocnt			インクリメント
	i		->lrecl			writeする長さ，不足分はpaddingで埋める．
	i		length		writeするbufの長さ
	i		padding		lrecl - lengthを埋めるキャラクタ
	
  注意
---------------------------------------------------------------------------- */
int		f_writeF(unsigned char *buf, int length, struct FileControl *fcntl, unsigned char padding)
{
	static	int	ret;
	int		pad_length;
	int		l;
	int		i; 

	fcntl->iocnt++;

	if( length <= fcntl->lrecl ) {
		l = length;
		pad_length = fcntl->lrecl - length;
	}
	else {
		l = fcntl->lrecl;
		pad_length = 0;
	}
	
	fwrite(buf, l, 1, fcntl->fpointer);

	for(i = 0; i < pad_length; i++) {
		fputc(padding , fcntl->fpointer);
	}

	return (ret = l);
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、bufをwriteする
		　IOCNT++する．
		  先頭２バイトにレコード長を設置し、３〜４バイトはヌル
		  fcntl->lreclに可変長のレコード長を設定する
戻値	：writeしたバイト数（可変長レコード長）
引数	：
	io		*fcntl		ファイルコントロール構造体
	o		->iocnt			インクリメント
	o		->lrecl			writeした長さ（可変長形式）
	i		length		writeするbufの長さ
注意
---------------------------------------------------------------------------- */
int		f_writeV(unsigned char *buf, int length, struct FileControl *fcntl)
{
	unsigned char	rdw[RDW_LENGTH];
	union {
		unsigned char	c[2];
		unsigned short	x;
	} ll;

	switch( fcntl->vbtype ) {
	case VB_HOST_BIG:
		ll.x = RDW_LENGTH + (unsigned short)length;
		rdw[0] = ll.c[1];					/* エンディアンをひっくり返して */
		rdw[1] = ll.c[0];
		rdw[2] = 0x00;
		rdw[3] = 0x00;
	
		fwrite(rdw, RDW_LENGTH, 1, fcntl->fpointer);		/* RDW */
	
		fcntl->iocnt++;
		fcntl->lrecl = ll.x;
		fwrite(buf, length, 1, fcntl->fpointer);			/* DATA */
		return fcntl->lrecl;
		break;
	case VB_NETC_LTL:
		ll.x = (unsigned short)length;
		rdw[0] = ll.c[0];					/* エンディアンはそのままで */
		rdw[1] = ll.c[1];
		rdw[2] = 0x00;
		rdw[3] = 0x00;

		fwrite(rdw, RDW_LENGTH, 1, fcntl->fpointer);		/* RDW	*/

		fcntl->iocnt++;
		fcntl->lrecl = ll.x;
		fwrite(buf, length, 1, fcntl->fpointer);			/* DATA */

		fwrite(rdw, RDW_LENGTH, 1, fcntl->fpointer);		/* RDW	*/

		return fcntl->lrecl;
		break;
	default:
		fprintf(stderr, "### ERR! f_writeV vbtype not defined ###\n");
		exit(-1);

	}
}

/* ----------------------------------------------------------------------------
処理	：構造体(struct fcntl)を受け取って、buf＋\nをwriteする
		　IOCNT++する．
		  bufは0x00で終端すること．
戻値	：writeしたバイト数(〜\nまでの長さ)
		  バイナリモードでオープンの時は，不正確・
引数	：
	io		*fcntl	ファイルコントロール構造体
	o		->iocnt		インクリメント

注意	改行は，ｵｰﾌﾟﾝのﾓｰﾄﾞに関係なく，[oxod,oxoa]の２バイト
		この関数を使う時は，ﾃｷｽﾄﾓｰﾄﾞでｵｰﾌﾟﾝしていること．
---------------------------------------------------------------------------- */
int		f_writeT(unsigned char *buf, struct FileControl *fcntl)
{
	static	int	ret;
	int		len;

	fcntl->iocnt++;
	len = strlen((char *)buf);
	if(len > 0)							/* 空行の時は￥ｎだけ出力 */
	{
		fwrite((char *)buf, len, 1, fcntl->fpointer);
	}

	switch(fcntl->cr)
	{
	case 0:
		return (ret = len);
	case 1:
		fputc(0x0a, fcntl->fpointer);
		return (ret = len + 1);
	case 2:
		fputc(0x0d, fcntl->fpointer);
		fputc(0x0a, fcntl->fpointer);
		return (ret = len + 2);
	default:
		return (ret = len);
	}
}

/* ----------------------------------------------------------------------------
処理	iniﾌｧｲﾙを初期化する．
		引数のcktblと突合し，cktblを設定する
戻値
引数
	i	inifile		iniﾌｧｲﾙをｱｸｾｽする構造体．->fname, ->fpointerが設定済のこと
	io	cktbl		iniﾌｧｲﾙのｴﾝﾄﾘｰを突合するTBL
注意
	CFGgetoptが便利
---------------------------------------------------------------------------- */
int		inifile_init(struct FileControl *inifile, struct Entry *cktbl)
{

	char	buf[MAXL_ENTRYBUF];
	struct Entry	*wk_cktbl;


	wk_cktbl = cktbl;
	while( ( entry_gets(buf, MAXL_ENTRYBUF, inifile->fpointer) != NULL )
					&&
		   ( strcmp(buf, "[END]") != 0 )
	     )
	{
		if(buf[0] == '[') {
			;
		}
		else {
			if( entry_chkset(buf, wk_cktbl) != 0) {
				return -1;
			}
			else {
				;
			}
		}
	}
	return 0;
}

/* ----------------------------------------------------------------------------
処理	:ini(sysin)ﾌｧｲﾙの入力ﾚｺｰﾄﾞﾁｪｯｸとcktbl[]のｾｯﾄ
戻値	:0（正常）／以外（エラー）
引数	:
	o	char	*line	:ﾁｪｯｸ対象ﾚｺｰﾄﾞ
注意	:「key=value」形式の入力であること
	CFGgetoptが便利
---------------------------------------------------------------------------- */
int 	entry_chkset(char *line, struct Entry *cktbl)
{
	char	*key, *value;
	struct Entry	*wk_pointer;

	wk_pointer = cktbl;
	key = strtok(line, " =\t\n");
	while(strcmp(wk_pointer->key,"") != 0) {
											/* keyが一致したら	*/
		if(strcmp(key, wk_pointer->key) == 0) {	
			wk_pointer->sitei = 1;
			value = strtok(NULL, " =\t\n");			/* 次のトークンは値	*/
			if( value != NULL) {
				strcpy(wk_pointer->value, value);	/* 値をセットして	*/
			} else {
				;				/* valueの指定なしは，初期値("")のまま	*/
			}
			return 0;								/* 関数を抜ける		*/
		}
											/* key不一致は、次のkey値でﾄﾗｲ */
		wk_pointer++;
	}
	return -1;								/* 最後まできたら不一致(ｴﾗｰ)*/
}

/* ----------------------------------------------------------------------------
処理	:ini形式(sysin)ﾌｧｲﾙから,１行読込
		 コメントの切捨てと，空行の読み飛ばしをして、
		 「key=value」のﾚｺｰﾄﾞを返却
戻値	:読み込んだ行へのポインタ
		 ＥＯＦはＮＵＬＬ
引数	:
	io	char 	*buf	:読み込んだ行へのﾎﾟｲﾝﾀ(ｺﾒﾝﾄ処理済)
	i	int		length	:bufの最大長
	i	FILE	*fp		:ﾌｧｲﾙﾎﾟｲﾝﾀ
注意	:strtok()を使ってｺﾒﾝﾄ処理をしているので「key=value」の間に
		 ｽﾍﾟｰｽを入れないこと。
		 bufの\0位置が変わっている。
		 入力 buf -> "first-token        ;    comment"
		 戻り buf -> "first-token"
履歴	2005/9/13
		コメントキャラを変更 ; -> ;#
			(cp = strchr(buf, ';')) != NULL
			(cp = strpbrk(buf, ";#")) != 0 
---------------------------------------------------------------------------- */
char	*entry_gets(char *buf, int length, FILE *fp)
{
	char	*ret, *cp;
	int		l;
	int		i;
	char	c;

	/*
	 *	１行先読み、ＥＯＦ（ＮＵＬＬ）は即リターン
	 *	 fgetsは￥ｎ付き
	 */
	if( (ret = fgets(buf, length, fp)) == NULL )	{ return NULL; }

	/* 最初に「;#\n」が出てくる位置に終端を置く */
	if( (cp = strpbrk(buf, ";#\n")) != 0 )			{ *cp = '\0'; }

	/*
	 *	空行か（ＮＵＬＬ），空行以外か
	 *	　空白・ＴＡＢ・￥ｎ以外が見つかれば、リターン
	 */
	 
	l = strlen(buf);
	for(i = 0; i < l; i++) {
		c = buf[i];
		if(c == ' ' || c == '\t') {
			;
		} else {
#ifdef	DBG_ENTRYGETS
			printf("entry_gets1/%s/\n", buf);
#endif
			return buf;
		}
	}
	
	/*
	 *	空行の時は，空行以外になるまで，次の行を読み続ける
	 */
	while( 1 ) {
		if( (ret = fgets(buf, length, fp)) == NULL )	{ return NULL; }	/* EOFでリターン */
		if( (cp = strpbrk(buf, ";#\n")) != 0 )			{ *cp = '\0'; }
		/*
		 *	空行かどうか
		 */
		l = strlen(buf);
		for(i = 0; i < l; i++) {
			c = buf[i];
			if(c == ' ' || c == '\t') {
				;
			} else {
#ifdef	DBG_ENTRYGETS
				printf("entry_gets1/%s/\n", buf);
#endif
				return buf;
			}
		}
	}
#ifdef	DBG_ENTRYGETS
	printf("entry_gets2/%s/\n", buf);
#endif
}

/* ----------------------------------------------------------------------------
処理	:fcntlのメンバをダンプ
戻値	:なし
引数	:
注意	:
---------------------------------------------------------------------------- */
int		fcntl_dmp(struct FileControl *fcntl, char *msg, FILE *fp)
{
	fprintf(fp, "%s\n", msg);
	fprintf(fp, "fname=%s\n", fcntl->fname);
	fprintf(fp, "iocnt=%ld\n", fcntl->iocnt);
	fprintf(fp, "recfm=%d\n", fcntl->recfm);
	fprintf(fp, "lrecl=%dn\n", fcntl->lrecl);
	fprintf(fp, "blksz=%d\n", fcntl->blksize);
	fprintf(fp, "eof  =%d\n", fcntl->eof);
	fprintf(fp, "bklp =%d\n", fcntl->blkp);
	fprintf(fp, "cr   =%d\n", fcntl->cr);
	fprintf(fp, "delim=%c\n", fcntl->delimiter);
	fprintf(fp, "recfm=%s\n", fcntl->s_recfm);
	fprintf(fp, "mode =%s\n", fcntl->mode);
	fprintf(fp, "han  =%s\n", fcntl->s_code_han);
	fprintf(fp, "zen  =%s\n", fcntl->s_code_zen);
	fprintf(fp, "vb   =%s\n", fcntl->s_vbtype);
	
	return 0;

}



#else
/* ----------------------------------------------------------------------------
	外部変数(static)の宣言
---------------------------------------------------------------------------- */
 
#endif		/* ifdef	MAIN *//Users/hiroshi/Google ドライブ/sync/MySocCurrent/btdtcnv-utf8/codecnv2.h

#endif		/* ifndef	_INC_FILECNTL */
