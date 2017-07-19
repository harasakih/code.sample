/* ----------------------------------------------------------------------------
ﾌｧｲﾙ	ファイルを扱うためのヘッダーファイル
---------------------------------------------------------------------------- */
#ifndef		_INC_FILECNTL
#define		_INC_FILECNTL

#include	<stdio.h>
#include	<string.h>
#include	<assert.h>

#define		MAXL_FILENAME	128		/* ファイル名バッファの最大長			*/
#define		MAXL_ENTRY		12		/* iniﾌｧｲﾙで[ｴﾝﾄﾘ名]の最大長			*/
#define		MAXL_KEY		12		/* 　〃       keyの最大長				*/
#define		MAXL_VALUE		128		/* 　〃       valueの最大長				*/
#define		MAXL_ENTRYBUF	256		/* 　〃     行バッファの最大長			*/

#define		FREAD_EOF		(-1)	/* fread()でのEOFはfeof()で判定するが	*/
									/* EOFの時はゼロ以外，notEOFは０		*/

#define		FCNTL_EOF		(-1)	/* FileControl.eofはeof(-1)とする		*/

#define		RDW_LENGTH		4		/* 可変長形式のレコード長部の長さ		*/
#define		BDW_LENGTH		4		/* ブロック化形式のブロック長部の長さ	*/

#define		RECFM_F			0		/* FileControl.recfm=F					*/
#define		RECFM_FB		1		/* FileControl.recfm=FB					*/
#define		RECFM_V			2		/* FileControl.recfm=V					*/
#define		RECFM_VB		3		/* FileControl.recfm=VB					*/
#define		RECFM_T			4		/* FileControl.recfm=F		固定長+\n	*/
#define		RECFM_C			5		/* FileControl.recfm=C		CSVﾌｧｲﾙ		*/

#ifdef	UNIX
									/* UNIXの改行は 0x0a */
#define		f_crlf( fp )		fputc(0x0a, fp)
#else 
									/* MS-DOSの改行は 0x0d, 0x0a */
#define		f_crlf( fp )		{ fputc(0x0d, fp); fputc(0x0a, fp); }
#endif

/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */
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

int		f_read(char *buf, int length, struct FileControl *fcntl);
int		f_readF(char *buf, struct FileControl *fcntl);
int		f_readV(char *buf, struct FileControl *fcntl);
int		f_readT(char *buf, int maxl, struct FileControl *fcntl);
int		f_readBLK(struct FileControl *fcntl);

int		f_write(char *buf, int length, struct FileControl *fcntl);
int		f_writeF(char *buf, int length, struct FileControl *fcntl, char padding);
int		f_writeV(char *buf, int length, struct FileControl *fcntl);
int		f_writeT(char *buf, struct FileControl *fcntl);

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

	if((fp = fopen(fcntl->fname, mode)) != NULL) {
		fcntl->fpointer = fp;
		return 0;
	} else {
		return -1;
	}
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
	bufは¥x00で終端されていなくてもかまわない．
---------------------------------------------------------------------------- */
int		f_read(char *buf, int length, struct FileControl *fcntl)
{

	fread(buf, length, 1, fcntl->fpointer);
	if( feof(fcntl->fpointer) ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		fcntl->iocnt++;
		return length;
	}
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
int		f_readF(char *buf, struct FileControl *fcntl)
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
int		f_readV(char *buf, struct FileControl *fcntl)
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
		fread(buf, lrecl, 1, fcntl->fpointer);
		fcntl->lrecl = ll.x;
		fcntl->iocnt++;
		return fcntl->lrecl;
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
int		f_readT(char *buf, int maxl, struct FileControl *fcntl)
{
	int			lrecl;

	if( fgets(buf, maxl, fcntl->fpointer) == NULL ) {
		fcntl->eof = FCNTL_EOF;
		return FREAD_EOF;
	}
	else {
		fcntl->iocnt++;
		lrecl = strlen( buf );
		return lrecl;
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
	char		bdw[BDW_LENGTH];
	union	{
		char	c[2];
		short	x;
	} ll;

/*
	assert( sizeof(short) == 2  );
*/

	/*
	 *	BLK部を読んでブロック長を算出
	 */
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
int		f_readVB(char *buf, struct FileControl *fcntl)
{
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
		return lrecl;
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
int		f_write(char *buf, int length, struct FileControl *fcntl)
{
	fcntl->iocnt++;
	fwrite(buf, length, 1, fcntl->fpointer);
	return length;
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
int		f_writeF(char *buf, int length, struct FileControl *fcntl, char padding)
{
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
		fwrite(&padding , 1, 1, fcntl->fpointer);
	}

	return l;
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
int		f_writeV(char *buf, int length, struct FileControl *fcntl)
{
	char	rdw[RDW_LENGTH];
	union {
		char	c[2];
		short	x;
	} ll;

/*
	assert( sizeof(short) == 2 );
*/

	ll.x = RDW_LENGTH + (short)length;
	rdw[0] = ll.c[1];
	rdw[1] = ll.c[0];
	rdw[2] = 0x00;
	rdw[3] = 0x00;

	fwrite(rdw, RDW_LENGTH, 1, fcntl->fpointer);

	fcntl->iocnt++;
	fcntl->lrecl = ll.x;
	fwrite(buf, length, 1, fcntl->fpointer);
	return fcntl->lrecl;
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
int		f_writeT(char *buf, struct FileControl *fcntl)
{
	int		len;

	fcntl->iocnt++;
	len = strlen(buf);
	if(len > 0)							/* 空行の時は￥ｎだけ出力 */
	{
		fwrite(buf, len, 1, fcntl->fpointer);
	}
	f_crlf(fcntl->fpointer);
	return len + 2;
}

/* ----------------------------------------------------------------------------
処理	iniﾌｧｲﾙを初期化する．
		引数のcktblと突合し，cktblを設定する
戻値
引数
	i	inifile		iniﾌｧｲﾙをｱｸｾｽする構造体．->fname, ->fpointerが設定済のこと
	io	cktbl		iniﾌｧｲﾙのｴﾝﾄﾘｰを突合するTBL
注意
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
---------------------------------------------------------------------------- */
int entry_chkset(char *line, struct Entry *cktbl)
{
	char	*key, *value;
	struct Entry	*wk_pointer;

	wk_pointer = cktbl;
	key = strtok(line, " =¥t\n");
	while(strcmp(wk_pointer->key,"") != 0) {
											/* keyが一致したら	*/
		if(strcmp(key, wk_pointer->key) == 0) {
			wk_pointer->sitei = 1;
			value = strtok(NULL, " =¥t\n");			/* 次のトークンは値	*/
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
---------------------------------------------------------------------------- */
char	*entry_gets(char *buf, int length, FILE *fp)
{
	char	*ret, *cp;
	char	*token;

	/*
	 *	１行先読み
	 *	ＥＯＦは即リターン
	 */
	ret = fgets(buf, length, fp);
	if(ret == NULL) {				/* EOFの時 */
		return NULL;
	}
									/* コメントの処理 */
	if( (cp = strchr(buf, ';')) != NULL )
	{								/* ;の位置を調べてあったら */
		*cp = '\0';					/* ;の位置には\0を代入 */
	}

	/*
	 *	空行か（ＮＵＬＬ），空行以外か
	 *	　トークンが見つからないとき strtok() はNULLを返す
	 */
	if( (token = strtok(buf, " ¥t\n")) != NULL )
	{
		return ret;
	}

	/*
	 *	空行の時は，空行以外になるまで，次の行を読み続ける
	 */
	while(token == NULL) {
		/*
		 *	１行読んで
		 *	ＥＯＦは即リターン
		 */
		ret = fgets(buf, length, fp);
		if(ret == NULL) {
			return NULL;
		}
		/*
		 *	コメントを処理して
		 */
		if( (cp = strchr(buf, ';')) != NULL )
		{
			*cp = '\0';
		}
		/*
		 *	空行かどうか
		 *	　空行以外はBreakして戻る
		 *	　空行は次の読込
		 */
		if( (token = strtok(buf, " ¥t\n")) != NULL)
		{
			break;
		}
	}
	return ret;
}


#else
/* ----------------------------------------------------------------------------
	外部変数(static)の宣言
---------------------------------------------------------------------------- */

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_FILECNTL */
