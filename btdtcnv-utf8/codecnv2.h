/*
 ***********************************************************************
 *	$Id: codecnv2.h,v 1.3 2006/07/17 07:25:30 hiroshi Exp $
 ***********************************************************************
 *	$Log: codecnv2.h,v $
 *	Revision 1.3  2006/07/17 07:25:30  hiroshi
 *	*** empty log message ***
 *	
 *	Revision 1.2  2006/06/25 13:25:18  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 * file	：codecnv2
 * 機能	：btdtcnv.cのヘッダファイル。マクロなど
 * 履歴	2005/09/13	COBOLから移植開始
 *		2006/07/15	ZD対応。とりあえず、ＮＥＴＣＯＢ限定で対応
 *					+:4X / -:5X / ABS:3X /
 */

#ifndef		_INC_CODECNV2
#define		_INC_CODECNV2

/* ----------------------------------------------------------------------------
	ＩＮＣＬＵＤＥファイル
---------------------------------------------------------------------------- */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	"filecntl.h"		/* for entry_gets */

/* ----------------------------------------------------------------------------
	ＤＥＦＩＮＥ＆ＭＡＣＲＯ
---------------------------------------------------------------------------- */
#define		MAX_HANTBL				256
#define		MAX_ZENTBL				65536
#define		HANZEN_BUF				512

#define		setlower_half(x,c)		((x & 0xf0) | (c & 0x0f))		/* xの下位半バイトに入れる	*/
#define		setupper_half(x,c)		((c << 4)   | (x & 0x0f))		/* xの上位半バイトに入れる	*/
#define		getlower_half(x)		(x & 0x0f)						/* xの下位半バイト	*/
#define		getupper_half(x)		((x & 0xf0)>>4)					/* xの上位半バイト	*/
#define		putknj2( k )			{ putchar(k >> 8); putchar(k & 0x00ff); }

#define		_max(x,y)				(x > y ? x : y)
#define		_min(x,y)				(x > y ? y : x)

/*
#define		numck( n )				((n < 0 || n > 9) ? 0 : n)
*/
#define		numck( n )				((n > 9) ? 0 : n)


#define		CODE_HAN_UNDEF			0
#define		CODE_ASC				1
#define		CODE_EBC				2
#define		MAX_CODE_HAN			3

#define		CODE_ZEN_UNDEF			0
#define		CODE_JIS83				1
#define		CODE_SJIS				2
#define		CODE_KEIS83				3
#define		CODE_JEF83				4
#define		MAX_CODE_ZEN			5

/* --------------------------------------------------------------------- */
#define		ASC_CHAR_ZERO			0x30
#define		ASC_CHAR_ONE			0x31
#define		ASC_CHAR_TWO			0x32
#define		ASC_CHAR_THREE			0x33
#define		ASC_CHAR_FOUR			0x34
#define		ASC_CHAR_FIVE			0x35
#define		ASC_CHAR_SIX			0x36
#define		ASC_CHAR_SEVEN			0x37
#define		ASC_CHAR_EIGHT			0x38
#define		ASC_CHAR_NINE			0x39
#define		ASC_CHAR_SML_A			0x61	/*	'a'	*/
#define		ASC_CHAR_SML_F			0x66	/*	'f'	*/
#define		ASC_CHAR_BIG_A			0x41	/*	'A'	*/
#define		ASC_CHAR_BIG_F			0x46	/*	'F'	*/
#define		ASC_CHAR_SPACE			0x20	/*	' '	*/
#define		ASC_CHAR_PLUS			0x2b	/*	'+'	*/
#define		ASC_CHAR_MINUS			0x2d	/*	'-'	*/
#define		ASC_CHAR_STAR			0x2a	/*	'*'	*/					/* 2006/07/15 */

/* --------------------------------------------------------------------- */
#define		ASC_ZD_PLUS				0x04
#define		ASC_ZD_MINUS			0x05
#define		ASC_ZD_ABS				0x03

/* --------------------------------------------------------------------- */
#define		EBC_CHAR_ZERO			0xf0
#define		EBC_CHAR_ONE			0xf1
#define		EBC_CHAR_TWO			0xf2
#define		EBC_CHAR_THREE			0xf3
#define		EBC_CHAR_FOUR			0xf4
#define		EBC_CHAR_FIVE			0xf5
#define		EBC_CHAR_SIX			0xf6
#define		EBC_CHAR_SEVEN			0xf7
#define		EBC_CHAR_EIGHT			0xf8
#define		EBC_CHAR_NINE			0xf9
#define		EBC_CHAR_SML_A			0x81	/*	'a'	*/
#define		EBC_CHAR_SML_F			0x86	/*	'f'	*/
#define		EBC_CHAR_BIG_A			0xc1	/*	'A'	*/
#define		EBC_CHAR_BIG_F			0xc6	/*	'F'	*/
#define		EBC_CHAR_SPACE			0x40	/*	' '	*/
#define		EBC_CHAR_PLUS			0x4e	/*	'+'	*/
#define		EBC_CHAR_MINUS			0x60	/*	'-'	*/
#define		EBC_CHAR_STAR			0x5c	/*	'*'	*/					/* 2006/07/15 */

/* --------------------------------------------------------------------- */
#define		EBC_ZD_PLUS				0x0c
#define		EBC_ZD_MINUS			0x0d
#define		EBC_ZD_ABS				0x0f

/* --------------------------------------------------------------------- */
#define		JIS83_CHAR_ZENSP		0x2121
#define		SJIS_CHAR_ZENSP			0x8140
#define		KEIS83_CHAR_ZENSP		0xa1a1
#define		JEF83_CHAR_ZENSP		0x4040

#define		CHAR_LOWVALUE			0x00

#define		SIGN_PLUS				0x0c
#define		SIGN_MINUS				0x0d




/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */
struct	CodeConvCTL	{
	char	tbl_han_path[256];
	char	tbl_zen_path[256];
	int		abend_han;
	int		abend_zen;
	int		replace_han;				/* cfgetoptsのため、ｉｎｔを使う */
	unsigned short		replace_zen;
} ;

/* ----------------------------------------------------------------------------
	関数プロトタイプ宣言
---------------------------------------------------------------------------- */
int		btch2hx (unsigned char *x,  int x_bytes,  unsigned char *hx,   int hx_bytes,  int ccode);
int		bthx2hx (unsigned char *x,  int x_bytes,  unsigned char *hx,   int hx_bytes,  int ccode);
int		bthex2x (unsigned char *x,  int x_bytes,  unsigned char *dmp,  int dmp_bytes, int ccode);
int		btx2hex (unsigned char *ch, int ch_bytes, unsigned char *hx,   int hx_bytes,  int ccode);
int		btch2pd (unsigned char *x,  int x_bytes,  unsigned char *pd,   int pd_bytes,  int ccode);
int		btpd2hs (unsigned char *pd, int pd_bytes, unsigned char *hs ,  int hs_bytes,  int ccode);
int		btpd2pd (unsigned char *pd, int pd_bytes, unsigned char *pd2,  int pd2_bytes, int ccode);
int		btbit2c (unsigned char *x,  int x_bytes,  unsigned char *ch,   int ch_bytes,  int ccode);
int		btpd2int(unsigned char *pd, int pd_bytes, int *value);
int		bthancnv(unsigned char *buf,int buf_bytes,unsigned char *otbuf,int otbuf_bytes, int ccode);
int		btzencnv(unsigned char *buf, int buf_bytes, unsigned char *otbuf, int otbuf_bytes, int ccode);
int		btzd2hs (unsigned char *zd, int zd_bytes, unsigned char *hs ,  int hs_bytes,  int ccode);

int		zentbl_init(struct CodeConvCTL codeconvctl);
int		hantbl_init(struct CodeConvCTL codeconvctl);
int		code_dmp2(char *arg);
void	code_const_init(void);
int		codeconvctl_dmp(struct CodeConvCTL ctl, char *msg, FILE *fp);

#ifdef  MAIN
/* ----------------------------------------------------------------------------
	外部変数の定義
---------------------------------------------------------------------------- */
	static	int				gin_CodeHan;
	static	int				gin_CodeZen;
	static	int				got_CodeHan;
	static	int				got_CodeZen;
	static	int				gChar[MAX_CODE_HAN][10];
	static	unsigned char	gChar_Sml_a[MAX_CODE_HAN], gChar_Sml_f[MAX_CODE_HAN];
	static	unsigned char	gChar_Big_A[MAX_CODE_HAN], gChar_Big_F[MAX_CODE_HAN];
	static	unsigned char	gChar_Space[MAX_CODE_HAN];
	static	unsigned char	gChar_Minus[MAX_CODE_HAN], gChar_Plus[MAX_CODE_HAN];
	static	unsigned char	gChar_Star[MAX_CODE_HAN];					/* 2006/07/15 */
	static	int				gZd_Plus[MAX_CODE_HAN];						/* 2006/07/15 */
	static	int				gZd_Minus[MAX_CODE_HAN];					/* 2006/07/15 */
	static	int				gZd_Abs[MAX_CODE_HAN];						/* 2006/07/15 */
	
	static	unsigned char	gStr_Numeric[MAX_CODE_HAN][11];
	static	unsigned short 	gChar_ZenSpace[MAX_CODE_ZEN];
	static 	unsigned char	han_tbl[MAX_HANTBL];
	static	unsigned short	zen_tbl[MAX_ZENTBL];


/* ----------------------------------------------------------------------------
	関数定義実態
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
	ASCII->HEX(そのままコピー、初期値は空白)
---------------------------------------------------------------------------- */
int		btch2hx(unsigned char *x, int x_bytes, unsigned char *hx, int hx_bytes, int ccode)
{
	int		l1;		/* x -> hx へコピーする長さ		*/
	int		l2;		/* のこりをクリアする長さ		*/

	
	l1	=	_min(x_bytes, hx_bytes);
	l2	=	_max(hx_bytes - x_bytes, 0);
	
	memcpy(hx, x, l1);
	if(l2 > 0) { memset(hx + l1, gChar_Space[ccode], l2);	}		/* space clear	*/
	
	return hx_bytes;
}

/* ----------------------------------------------------------------------------
	HEX->HEX(そのままコピー、初期値はＬＯＷ−ＶＡＬＵＥ)
---------------------------------------------------------------------------- */
int		bthx2hx(unsigned char *x, int x_bytes, unsigned char *hx, int hx_bytes, int ccode)
{
	int		l1;			/* x -> hx へコピーする長さ		*/
	int		l2;			/* hxのあまりをクリアする長さ	*/
	
	l1 = _min(x_bytes, hx_bytes);
	l2 = _max(hx_bytes - x_bytes, 0);
	
	memcpy(hx, x, l1);
	if(l2 > 0) { memset(hx + l1, CHAR_LOWVALUE, l2);	}		/* low-value clear	*/
	return hx_bytes;
}

/* ----------------------------------------------------------------------------
	ASCII->PACK変換(符号付きﾃｷｽﾄ数字を変換する)
---------------------------------------------------------------------------- */
int		btch2pd(unsigned char *ascii, int asc_bytes, unsigned char *pack, int pack_bytes, int ccode)
{
	int				num_first;			/* 空白，符号以外（数字）の最初の位置	*/
	int				keta;				/* 数値の桁数（符号を含まない）			*/
	int				keta_idx;			/* asciiを走査するIDX					*/
	int				half_idx;			/* 出力を1/2ﾊﾞｲﾄづつ走査するIDX			*/
	int				pack_idx;			/* 出力ＰＡＣＫを１バイト走査するIDX	*/
	unsigned char	a_digit;			/* 入力の数値一桁						*/
	unsigned char	*p_digit;			/* PACK変換中のASC文字へのポインタ		*/
	unsigned char	sign;				/* 数値の符号(C:+,D:-)					*/
	unsigned char	x;

	
										/* 0-9が最初に表れる位置				*/
	num_first = strcspn( (char *)ascii, (char *)gStr_Numeric[ccode] );
										/* 0-9のみの部分文字列長：
										   0-9以外の文字が表れる位置			*/
	keta      = strspn( (char *)(ascii + num_first), (char *)(gStr_Numeric[ccode]) );

	if( ascii[num_first - 1] == gChar_Minus[ccode] ) {
		sign = SIGN_MINUS;
	} else {
		sign = SIGN_PLUS;
	}
	
	memset(pack, CHAR_LOWVALUE, pack_bytes);				/* low-value clear	*/
	x 					 = pack[pack_bytes - 1];
	pack[pack_bytes - 1] = (char)setlower_half(x, sign);	/* 符号のセット		*/

	p_digit	=	ascii + num_first + keta - 1;
	for(keta_idx = keta - 1,half_idx = 2*pack_bytes - 2;
	    keta_idx >= 0 && half_idx >= 0;
	    keta_idx--, half_idx--)
	{
		a_digit = (unsigned char)(*p_digit - gChar[ccode][0]);
		pack_idx = half_idx / 2;

		x = pack[pack_idx];
		if(half_idx % 2 == 0) {
										/* 偶数桁は上位半バイト					*/
			pack[pack_idx] = (unsigned char)setupper_half(x, a_digit);
		} else {
										/* 奇数桁は下位半バイト					*/
			pack[pack_idx] = (unsigned char)setlower_half(x, a_digit);
		}
		
		p_digit--;
		
	}
	
	return pack_bytes;
}

/* ----------------------------------------------------------------------------
	HexDump
---------------------------------------------------------------------------- */
int 	bthex2x(unsigned char *data, int data_bytes, unsigned char *dmp, int dmp_bytes, int ccode)
{
	int			l1;		/* data -> dmp へＨＥＸ−ＤＭＰする長さ		*/
	int			l2;		/* のこりをクリアする長さ					*/
	int			i;

	l1	=	_min(data_bytes * 2, dmp_bytes);
	l2	=	_max(dmp_bytes - data_bytes * 2, 0);

	for(i = 0; i < l1; i++){
		sprintf( (char *)(dmp + i*2), "%02x", data[i]);
	}
	if(l2 > 0) { memset(dmp + l1, gChar_Space[ccode], l2);	}			/* space clear	*/
	return dmp_bytes;
}

/* ----------------------------------------------------------------------------
	PACK->マイナス編集

2005/10/18	numck追加、４ビットが０〜９以外は無理やりゼロ
---------------------------------------------------------------------------- */
int		btpd2hs(unsigned char *pd, int pd_bytes, unsigned char *hs, int hs_bytes, int ccode)
{
	int				i;
	unsigned char	one_byte;
	unsigned char	left, right;

	unsigned char	sign;
	unsigned char	a_pd;
	int				l1, l2;		/* l1:hsを設定する長さ、l2:のこりをクリアする長さ	*/
	
	
	
	if(hs_bytes < 2 * pd_bytes) {
		fprintf(stderr, "btpd2hs:output length is short\n");
		exit(-1);
	}
	

	l1	=	_min(pd_bytes * 2, hs_bytes);
	l2	=	_max(hs_bytes - pd_bytes * 2, 0);

	a_pd		= pd[pd_bytes - 1];
	if( getlower_half( a_pd ) == SIGN_PLUS ){
		sign	=	gChar_Plus[ccode];
	} else {
		sign	=	gChar_Minus[ccode];
	}
/*
	for(hs_idx = hs_bytes - 1, half_idx = pd_bytes * 2 - 2;
		hs_idx >= 0 && half_idx >=0;
		hs_idx--, half_idx--)
	{
		pd_idx	=	half_idx / 2;
		a_pd	=	pd[pd_idx];

		if((half_idx %2) == 0) {
			hs[hs_idx]	=	(char)(getupper_half( a_pd ) + gChar[ccode][0]);
		} else {
			hs[hs_idx]	=	(char)(getlower_half( a_pd ) + gChar[ccode][0]);
		}
	}
	hs[hs_idx] = sign;
	return hs_bytes;
*/

	hs[0]			=	sign;
	for(i = 0; i < pd_bytes - 1; i++)
	{
		one_byte	=	pd[i];
		left		=	one_byte >> 4;
		right		=	one_byte & 0x0f;
		
		left		= numck( left );
		right		= numck( right );
		hs[i*2 + 1]	=	left + gChar[ccode][0];
		hs[i*2 + 2]	=	right + gChar[ccode][0];
	}
	
	one_byte	=	pd[pd_bytes - 1];
	left		=	one_byte >> 4;
	left		=	numck( left );
	hs[hs_bytes - 1] = left + gChar[ccode][0];

	if(l2 > 0) { memset(hs + l1, gChar_Space[ccode], l2);	}		/* space clear	*/
	return hs_bytes;

}

/* ----------------------------------------------------------------------------
	PACK->PACK
---------------------------------------------------------------------------- */
int		btpd2pd(unsigned char *pd, int pd_bytes, unsigned char *pd2, int pd2_bytes, int ccode)
{

	int		diff;
	
	diff	=	pd2_bytes - pd_bytes;
	if(diff < 0) {
		fprintf(stderr, "btpd2pd:dest-length is short\n");
		exit(-1);
	}
	
	
	memcpy(pd2 + diff, pd, pd_bytes);
	memset(pd2, CHAR_LOWVALUE, diff);

	return pd2_bytes;
}

/* ----------------------------------------------------------------------------
	ASCII->HEX(POKE)
---------------------------------------------------------------------------- */
int		btx2hex(unsigned char *ch, int ch_bytes, unsigned char *hx, int hx_bytes, int ccode)
{
	int				ch_idx;
	int				hx_idx;
	unsigned char	byte;
	unsigned char	halfu;
	unsigned char	halfl;
	unsigned char	a_char;
	
	if(ch_bytes > 2 * hx_bytes) {
		fprintf(stderr, "btx2hex:dest-length is short\n");
		exit(-1);
	}
	
	for(ch_idx = 0; ch_idx < ch_bytes; ch_idx++, ch_idx++){
		a_char = ch[ch_idx];
		if(gChar[ccode][0] <= a_char && a_char <= gChar[ccode][9]) {
			halfu = a_char - gChar[ccode][0];
		}
		else if(gChar_Sml_a[ccode] <= a_char && a_char <= gChar_Sml_f[ccode]) {
			halfu = a_char - gChar_Sml_a[ccode] + 10;
		}
		else if(gChar_Big_A[ccode] <= a_char && a_char <= gChar_Big_F[ccode]) {
			halfu = a_char - gChar_Big_A[ccode] + 10;
		}
		else {
			fprintf(stderr, "btx2hex:hex-char not 0-9a-fA-F/%2x/\n", a_char);
			exit(-1);
		}

		a_char = ch[ch_idx + 1];
		if(gChar[ccode][0] <= a_char && a_char <= gChar[ccode][9]) {
			halfl = a_char - gChar[ccode][0];
		}
		else if(gChar_Sml_a[ccode] <= a_char && a_char <= gChar_Sml_f[ccode]) {
			halfl = a_char - gChar_Sml_a[ccode] + 10;
		}
		else if(gChar_Big_A[ccode] <= a_char && a_char <= gChar_Big_F[ccode]) {
			halfl = a_char - gChar_Big_A[ccode] + 10;
		}
		else {
			fprintf(stderr, "btx2hex:hex-char not 0-9a-fA-F/%2x/\n", a_char);
			exit(-1);
		}
		
		hx_idx = ch_idx / 2;
		byte = (halfu << 4) | (0x0f & halfl);
		hx[hx_idx] = byte;
	}
	return hx_bytes;
}

/* ----------------------------------------------------------------------------
	PACK -> int
---------------------------------------------------------------------------- */
int		btpd2int(unsigned char *pack, int pack_bytes, int *binary)
{
	int				sign;				/* 符号(C:1, D:-1) */
	int				i;
	int				work;				/* 変換ワーク */
	unsigned char	one_byte;			/* PACK->Binary変換中の１バイト */
	unsigned char	left;				/* 変換中１バイトの左半バイト */
	unsigned char	right;				/* 変換中１バイトの右半バイト */

	sign = (pack[pack_bytes - 1] & 0x01) * (-2) + 1;

	work = 0;
	for(i = 0; i <  pack_bytes - 1; i++){
		one_byte = (int)pack[i];
		left  = one_byte >> 4;
		right = one_byte & 0x0f;
										/* ２桁づつ変換 */
		work = work * 100 + left * 10 + right;
	}

	one_byte = (int)pack[pack_bytes - 1];
	left = one_byte >> 4;
	work = work * 10 + left;
	*binary = work * sign;

	return *binary;
}

/* ----------------------------------------------------------------------------
	半角変換
---------------------------------------------------------------------------- */
int		bthancnv(unsigned char *buf, int buf_bytes, unsigned char *otbuf, int otbuf_bytes, int ccode)
{
	int		i;
	for(i = 0; i < buf_bytes; i++)
	{
		otbuf[i]	= han_tbl[ (int)buf[i] ];
	}

	return otbuf_bytes;
}
	
/* ----------------------------------------------------------------------------
	全角変換
---------------------------------------------------------------------------- */
int		btzencnv(unsigned char *buf, int buf_bytes, unsigned char *otbuf, int otbuf_bytes, int ccode)
{
	int		i;
	int		zen1, zen2;

	for(i = 0; i < buf_bytes; i++, i++)
	{
		zen1		= ((int)buf[i] << 8) | (int)buf[i+1];
		zen2		= zen_tbl[ zen1 ];
		otbuf[i]	= (unsigned char)(zen2 >> 8);
		otbuf[i+1] 	= (unsigned char)(zen2 & 0x00ff);
	}
	
	return otbuf_bytes;
}

/* ----------------------------------------------------------------------------
	ビット→キャラクタ
---------------------------------------------------------------------------- */
int		btbit2c (unsigned char *x,  int x_bytes,  unsigned char *ch,   int ch_bytes,  int ccode)
{
	static char			bittbl[16][5] = 
	{ "0000","0001","0010","0011",
	  "0100","0101","0110","0111",
	  "1000","1001","1010","1011",
	  "1100","1101","1110","1111" };
	int				i;
	unsigned char	one_byte;
	unsigned char	left;
	unsigned char	right;
		
	for(i = 0; i < x_bytes; i++){
		one_byte = x[i];
		left  = one_byte >> 4;
		right = one_byte & 0x0f;
		
		ch[i*8]     = bittbl[left][0];
		ch[i*8 + 1] = bittbl[left][1];
		ch[i*8 + 2] = bittbl[left][2];
		ch[i*8 + 3] = bittbl[left][3];
	
		ch[i*8 + 4] = bittbl[right][0];
		ch[i*8 + 5] = bittbl[right][1];
		ch[i*8 + 6] = bittbl[right][2];
		ch[i*8 + 7] = bittbl[right][3];
	}
	return x_bytes * 8;
}


/* ----------------------------------------------------------------------------
	名前	：ZD -> 編集形式
	引数	：
		ccode	i : 出力文字コードが設定される。

	2006/07/15	作成開始：終了
---------------------------------------------------------------------------- */
int		btzd2hs(unsigned char *zd, int zd_bytes, unsigned char *hs, int hs_bytes, int ccode)
{
	char			sign;				/* 符号バイト*/
	int				i;
	int				digit;				/* 下半バイトを取り出す */

		 if( getupper_half(zd[zd_bytes - 1]) == gZd_Plus[gin_CodeHan] )		{ sign	= gChar_Plus[ccode];  }
	else if( getupper_half(zd[zd_bytes - 1]) == gZd_Minus[gin_CodeHan] )	{ sign	= gChar_Minus[ccode]; }
	else if( getupper_half(zd[zd_bytes - 1]) == gZd_Abs[gin_CodeHan] )		{ sign	= gChar_Space[ccode];   }
	else																	{ sign	= gChar_Star[ccode]; }

/* 
	switch(gin_CodeHan)
	{
	case CODE_ASC:
			 if( getupper_half(zd[zd_bytes - 1]) == 0x4 )	{ sign	= gChar_Plus[ccode]; }
		else if( getupper_half(zd[zd_bytes - 1]) == 0x5 )	{ sign	= gChar_Minus[ccode]; }
		else if( getupper_half(zd[zd_bytes - 1]) == 0x3 )	{ sign	= gChar_Space[ccode]; }
		else												{ sign	= gChar_Star[ccode]; }
		break;
	case CODE_EBC:
			 if( getupper_half(zd[zd_bytes - 1]) == 0xC )	{ sign	= gChar_Plus[ccode]; }
		else if( getupper_half(zd[zd_bytes - 1]) == 0xD )	{ sign	= gChar_Minus[ccode]; }
		else if( getupper_half(zd[zd_bytes - 1]) == 0xF )	{ sign	= gChar_Space[ccode]; }
		else												{ sign	= gChar_Star[ccode]; }
		break;
	default:
		fprintf(stderr, "btzd2hs:input char-code not defined\n");
		exit(-1);
	}
*/	
	
	
	hs[0]	= sign;									/* 最初は、符号記号							*/
	for(i = 0; i <  zd_bytes ; i++){
		digit	= (int)getlower_half( zd[i] );		/* 最後は、符号半バイトがあるのでコード変換不可 */
		hs[i+1]	= gChar[ccode][digit];
	}
	
	return hs_bytes;

}

/* ----------------------------------------------------------------------------
	名前	：ZD -> ZD
	引数	：
		ccode	i : 出力文字コードが設定される。

	2006/07/15	作成開始：終了
---------------------------------------------------------------------------- */
int		btzd2zd(unsigned char *zd, int zd_bytes, unsigned char *hs, int hs_bytes, int ccode)
{
	int				sign;				/* 符号バイト*/
	int				i;
	int				digit;				/* 下半バイトを取り出す */

		 if( getupper_half(zd[zd_bytes - 1]) == gZd_Plus[gin_CodeHan] )		{ sign	= gZd_Plus[ccode];  }
	else if( getupper_half(zd[zd_bytes - 1]) == gZd_Minus[gin_CodeHan] )	{ sign	= gZd_Minus[ccode]; }
	else if( getupper_half(zd[zd_bytes - 1]) == gZd_Abs[gin_CodeHan] )		{ sign	= gZd_Abs[ccode];   }
	else																	{ sign	= 0; }
	
	for(i = 0; i <  zd_bytes ; i++){
		digit	= (int)getlower_half( zd[i] );		/* 最後は、符号半バイトがあるのでコード変換不可 */
		hs[i]	= gChar[ccode][digit];
	}
	hs[hs_bytes - 1] = setupper_half(hs[hs_bytes - 1], sign);
	
	return hs_bytes;

}


/* ----------------------------------------------------------------------------
	文字コード定数の初期設定
---------------------------------------------------------------------------- */
void	code_const_init()
{
	int	i;

	/*	ASC		*/
		gChar[CODE_ASC][0]		=	ASC_CHAR_ZERO	;
		gChar[CODE_ASC][1]		=	ASC_CHAR_ONE	;
		gChar[CODE_ASC][2]		=	ASC_CHAR_TWO	;
		gChar[CODE_ASC][3]		=	ASC_CHAR_THREE	;
		gChar[CODE_ASC][4]		=	ASC_CHAR_FOUR	;
		gChar[CODE_ASC][5]		=	ASC_CHAR_FIVE	;
		gChar[CODE_ASC][6]		=	ASC_CHAR_SIX	;
		gChar[CODE_ASC][7]		=	ASC_CHAR_SEVEN	;
		gChar[CODE_ASC][8]		=	ASC_CHAR_EIGHT	;
		gChar[CODE_ASC][9]		=	ASC_CHAR_NINE	;

		gChar_Sml_a[CODE_ASC]	=	ASC_CHAR_SML_A	;
		gChar_Sml_f[CODE_ASC]	=	ASC_CHAR_SML_F	;
		gChar_Big_A[CODE_ASC]	=	ASC_CHAR_BIG_A	;
		gChar_Big_F[CODE_ASC]	=	ASC_CHAR_BIG_F	;
		gChar_Space[CODE_ASC]	=	ASC_CHAR_SPACE	;
		
		gChar_Minus[CODE_ASC]	=	ASC_CHAR_MINUS	;
		gChar_Plus[CODE_ASC]	=	ASC_CHAR_PLUS	;
		gChar_Star[CODE_ASC]	=	ASC_CHAR_STAR	;					/* 2006/07/15 */

		gZd_Plus[CODE_ASC]		=	ASC_ZD_PLUS		;					/* 2006/07/15 */
		gZd_Minus[CODE_ASC]		=	ASC_ZD_MINUS	;					/* 2006/07/15 */
		gZd_Abs[CODE_ASC]		=	ASC_ZD_ABS		;					/* 2006/07/15 */

		
	for(i = 0; i < 10; i++)
	{
		gStr_Numeric[CODE_ASC][i]	=	gChar[CODE_ASC][i];
	}
		gStr_Numeric[CODE_ASC][10]	=	'\0';

	/*	EBCDIK		*/
		gChar[CODE_EBC][0]		=	EBC_CHAR_ZERO	;
		gChar[CODE_EBC][1]		=	EBC_CHAR_ONE	;
		gChar[CODE_EBC][2]		=	EBC_CHAR_TWO	;
		gChar[CODE_EBC][3]		=	EBC_CHAR_THREE	;
		gChar[CODE_EBC][4]		=	EBC_CHAR_FOUR	;
		gChar[CODE_EBC][5]		=	EBC_CHAR_FIVE	;
		gChar[CODE_EBC][6]		=	EBC_CHAR_SIX	;
		gChar[CODE_EBC][7]		=	EBC_CHAR_SEVEN	;
		gChar[CODE_EBC][8]		=	EBC_CHAR_EIGHT	;
		gChar[CODE_EBC][9]		=	EBC_CHAR_NINE	;

		gChar_Sml_a[CODE_EBC]	=	EBC_CHAR_SML_A	;
		gChar_Sml_f[CODE_EBC]	=	EBC_CHAR_SML_F	;
		gChar_Big_A[CODE_EBC]	=	EBC_CHAR_BIG_A	;
		gChar_Big_F[CODE_EBC]	=	EBC_CHAR_BIG_F	;
		gChar_Space[CODE_EBC]	=	EBC_CHAR_SPACE	;

		gChar_Minus[CODE_EBC]	=	EBC_CHAR_MINUS	;
		gChar_Plus[CODE_EBC]	=	EBC_CHAR_PLUS	;
		gChar_Star[CODE_EBC]	=	EBC_CHAR_STAR	;					/* 2006/07/15 */

		gZd_Plus[CODE_EBC]		=	EBC_ZD_PLUS		;					/* 2006/07/15 */
		gZd_Minus[CODE_EBC]		=	EBC_ZD_MINUS	;					/* 2006/07/15 */
		gZd_Abs[CODE_EBC]		=	EBC_ZD_ABS		;					/* 2006/07/15 */

	for(i = 0; i < 10; i++)
	{
		gStr_Numeric[CODE_EBC][i]	=	gChar[CODE_EBC][i];
	}
		gStr_Numeric[CODE_EBC][10]	=	'\0';


		gChar_ZenSpace[CODE_JIS83]	=	JIS83_CHAR_ZENSP	;
		gChar_ZenSpace[CODE_SJIS]	=	SJIS_CHAR_ZENSP	;
		gChar_ZenSpace[CODE_KEIS83]	=	KEIS83_CHAR_ZENSP	;
		gChar_ZenSpace[CODE_JEF83]	=	JEF83_CHAR_ZENSP	;
		
}

/* ----------------------------------------------------------------------------
	JEF<->JIS, EBC<->ASCII変換テーブルのダンプ
---------------------------------------------------------------------------- */
int	code_dmp2(char *arg)
{
	int				i;
	unsigned short	mae, ato;
	int				from, to;


	if(strcmp(arg, "HANTBL") == 0)
	{
		printf("HANTBL\n");
		printf("in->ot=%d->%d\n", gin_CodeHan, got_CodeHan);
		
		for(i = 0; i < MAX_HANTBL; i++)
		{
			from = i;
			to   = han_tbl[from];
			if(from % 8 == 0) printf("\n");
			printf("(%c)%02x-%02x:", to, from, to);
		}
		printf("\n");
	}
	else if(strcmp(arg, "ZENTBL") == 0)
	{
		printf("ZENTBL\n");
		printf("in->ot=%d->%d\n", gin_CodeZen, got_CodeZen);
		
		for(i = 0; i < MAX_ZENTBL; i++)
		{
			mae = (unsigned short)i;
			ato = zen_tbl[mae];

			if(mae % 4 == 0) printf("\n");
			printf("%04x:%04x(", mae, ato );
			putknj2( ato );
			printf(") ");
		}
		printf("\n");
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	全角、半角変換テーブルのメモリ格納
---------------------------------------------------------------------------- */
int		zentbl_init(struct CodeConvCTL ctl)
{
	int				mae, ato;
	int				i;
	char			buf[HANZEN_BUF];

	
	FILE	*fp;
	if((fp = fopen(ctl.tbl_zen_path, "rt")) == NULL){
		fprintf(stderr, "Cannot open zen-tbl-file\n");
		exit(-1);
	}
	
	if(ctl.abend_zen == 0){
		for(i = 0; i < MAX_ZENTBL; i++) {
			zen_tbl[i]	=	(unsigned short)ctl.replace_zen;
		}
	}
	
	
	while( entry_gets(buf, HANZEN_BUF, fp) != NULL )
	{
		sscanf(buf, "%x,%x\n", &mae, &ato);
		zen_tbl[mae]	=	(unsigned short)ato;
	}
	fclose(fp);
	return 0;

}

int		hantbl_init(struct CodeConvCTL ctl)
{
	int		mae, ato;
	int		i;
	char	buf[HANZEN_BUF];
	
	FILE	*fp;
	if((fp = fopen(ctl.tbl_han_path, "rt")) == NULL){
		fprintf(stderr, "Cannot open han-tbl-file\n");
		exit(-1);
	}
	
	if(ctl.abend_han == 0){
		for(i = 0; i < MAX_HANTBL; i++) {
			han_tbl[i]	=	(unsigned char)ctl.replace_han;
		}
	}
	
	while( entry_gets(buf, HANZEN_BUF, fp) != NULL )
	{
		sscanf(buf, "%x,%x\n", &mae, &ato);
		han_tbl[mae]	=	(unsigned char)ato;
	}
	fclose(fp);
	return 0;

}

/* ----------------------------------------------------------------------------
	コード変換ＣＴＬのダンプ
---------------------------------------------------------------------------- */
int		codeconvctl_dmp(struct CodeConvCTL ctl, char *msg, FILE *fp)
{
	fprintf(fp, "%s\n", msg);
	fprintf(fp, "han_path =%s\n", ctl.tbl_han_path);
	fprintf(fp, "zen_path =%s\n", ctl.tbl_zen_path);
	fprintf(fp, "abend_han=%d\n", ctl.abend_han);
	fprintf(fp, "abend_zen=%d\n", ctl.abend_zen);
	fprintf(fp, "rep_han  =%c:%x\n", (unsigned char)ctl.replace_han, (unsigned char)ctl.replace_han);
	fprintf(fp, "rep_zen  =:%x\n",   ctl.replace_zen);
	return 0;
}

#else
/* ----------------------------------------------------------------------------
	外部変数(static)の宣言
---------------------------------------------------------------------------- */
extern	static	int				gin_CodeHan;
extern	static	int				gin_CodeZen;
extern	static	int				got_CodeHan;
extern	static	int				got_CodeZen;
extern	static	int				gChar[MAX_CODE_HAN][10];
extern	static	unsigned char	gChar_Sml_a[MAX_CODE_HAN], gChar_Sml_f[MAX_CODE_HAN];
extern	static	unsigned char	gChar_Big_A[MAX_CODE_HAN], gChar_Big_F[MAX_CODE_HAN];
extern	static	unsigned char	gChar_Space[MAX_CODE_HAN];
extern	static	unsigned char	gChar_Minus[MAX_CODE_HAN], gChar_Plus[MAX_CODE_HAN];
extern	static	unsigned char	gChar_Star[MAX_CODE_HAN];				/* 2006/07/15 */
extern	static	int				gZd_Plus[MAX_CODE_HAN];					/* 2006/07/15 */
extern	static	int				gZd_Minus[MAX_CODE_HAN];				/* 2006/07/15 */
extern	static	int				gZd_Abs[MAX_CODE_HAN];					/* 2006/07/15 */

extern	static	unsigned char	gStr_Numeric[MAX_CODE_HAN][11];
extern	static	unsigned short 	gChar_ZenSpace[MAX_CODE_ZEN];
extern	static 	unsigned char	han_tbl[MAX_HANTBL];
extern	static	unsigned short	zen_tbl[MAX_ZENTBL];

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_FILECNTL */

