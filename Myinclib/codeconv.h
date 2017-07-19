/* ----------------------------------------------------------------------------
ﾌｧｲﾙ	コード変換をするためのヘッダーファイル
		ASCII<->EBCK / ASCII<->EBCA
		JEF,SJIS,JIS
---------------------------------------------------------------------------- */
#ifndef		_INC_CODECONV
#define		_INC_CODECONV

/* ----------------------------------------------------------------------------
	ＩＮＣＬＵＤＥファイル
---------------------------------------------------------------------------- */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

/* ----------------------------------------------------------------------------
	ＤＥＦＩＮＥ＆ＭＡＣＲＯ
---------------------------------------------------------------------------- */

									/* 整数の内部表現をターゲットマシンで区別する */
#define		FACOM_MSP		1		/* Fujitsu MSP Host Computer */
#define		INTEL86			2		/* Intel 86x */
#define		MC68			3		/* Motorola 68xxx */

#ifdef		INT64					/*	４ﾊﾞｲﾄ、８ﾊﾞｲﾄ整数の型の定義 */
	typedef	long			Bin4;
	typedef	_int64			Bin8;
#else
	typedef	long			Bin4;
	typedef	long			Bin8;
#endif

#define		JEF_ZENSP		0x4040
#define		KEI_ZENSP		0xa1a1			/* 2005.08日立対応 */
#define		JIS_ZENSP		0x2121
#define		SJIS_ZENSP		0x8140
#define		MAX_JEFJISTBL	65536			/* JEF<->JIS変換ＴＢＬの項目数 2^16 */
#define		MAX_KEIJISTBL	MAX_JEFJISTBL	/* KEIS<->JIS変換ＴＢＬの項目数 2^16 */
#define		MAX_EBCASCTBL	256				/* EBC<->ASC変換ＴＢＬ */
#define		xswap( x )		((x << 8) | (x >> 8))
#define		putknj( k )		{ putchar(k >> 8); putchar(k & 0x00ff); }

#define		_jis2jef( x )	(jis2jef_tbl[x])
#define		_jef2jis( x )	(jef2jis_tbl[x])
#define		_jis2kei( x )	(jis2kei_tbl[x])		/* 2005.08日立対応 */
#define		_kei2jis( x )	(kei2jis_tbl[x])		/* 2005.08日立対応 */
#define		_ebce2asc( x )	(ebce2asc_tbl[x])
#define		_ebck2asc( x )	(ebck2asc_tbl[x])
#define		_ebch2asc( x )	(ebch2asc_tbl[x])		/* 2005.08日立対応 */
#define		_asc2ebce( x )	(asc2ebce_tbl[x])
#define		_asc2ebck( x )	(asc2ebck_tbl[x])
#define		_asc2ebch( x )	(asc2ebch_tbl[x])		/* 2005.08日立対応 */

#define		iskanji_sjis(c)	((((c)> 0x80)&&((c)<0xa0))||(((c)>=0xe0)&&((c)<0xfe)))
#define		lower_half(x,c)	((x & 0xf0) | (c & 0x0f))		/* xの下位半バイトに入れる	*/
#define		upper_half(x,c)	((c << 4)   | (x & 0x0f))		/* xの上位半バイトに入れる	*/
#define		kiriage(x)		(x == (int)x ? (int)x : (int)x + 1)

/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
	関数のプロトタイプ宣言
---------------------------------------------------------------------------- */
unsigned short	_jis2sjis(unsigned short jis);
unsigned short	_sjis2jis(unsigned short sjis);

int		jefjis_init(char *fname);
int		keijis_init(void);

int		code_dmp(char *arg);
int		ascebc_init(void);

int		pack2bin4(unsigned char *pack, int pack_bytes,
				Bin4 *binary, int *binary_bytes, int machine);

int		pack2bin8(unsigned char *pack, int pack_size,
				Bin8 *binary, int *binary_bytes, int machine);

int		pack2ascii(unsigned char *pack, int pack_bytes,
			   char *ascii, int *ascii_bytes, int machine);

int		hexdmp(unsigned char *data, int data_bytes,
				 char *dmp, int *dmp_bytes);

int		hexset(char *hex, int hex_bytes,
			 unsigned char *data, int *data_bytes);

int		bit2cbit(unsigned char *bit, int bit_bytes,
				char *charbit, int *charbit_bytes);

int		ebce2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes);

int		ebck2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes);
														/* 2005.08 日立対応 */
int		ebch2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes);

int		asc2ebce(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes);

int		asc2ebck(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes);
														/* 2005.08 日立対応 */
int		asc2ebch(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes);

int		jef12sjis(unsigned char *jef, int jef_bytes,
			 char *sjis, int *sjis_bytes);

int		jef22sjis(unsigned char *jef, int jef_bytes,
			 char *sjis, int *sjis_bytes);
														/* 2005.08 日立対応 */
int		kei22sjis(unsigned char *kei, int kei_bytes,
			 char *sjis, int *sjis_bytes);

int		bin22asc(unsigned char *binary2, int bin_bytes,
			 char *asc, int *asc_bytes, int machime);

int		bin42asc(unsigned char *binary4, int bin_bytes,
			 char *asc, int *asc_bytes, int machine);

int		bin82asc(unsigned char *binary8, int bin_bytes,
			              char *asc,     int *asc_bytes, int machine);

int		abit2cbit(unsigned char *bit, int bit_bytes,
		          unsigned char *cbit, int *cbit_bytes);

int		asc2pack( char *ascii, int asc_bytes,
		 unsigned char *pack,  int *pack_bytes);


int		asc2bit8( char *ascii, int asc_bytes,
		 unsigned char *bit,   int *bit_bytes);

int		sjis2jef2(unsigned char *sjis, int sjis_bytes,
		          unsigned char *jef,  int *jef_bytes);
														/* 2005.08 日立対応 */
int		sjis2kei2(unsigned char *sjis, int sjis_bytes,
		          unsigned char *kei,  int *kei_bytes);

#ifdef	MAIN

/* ----------------------------------------------------------------------------
	外部変数の定義
---------------------------------------------------------------------------- */

/*
 * 英小文字変換：ＥＢＣＥ
 */
	static unsigned char ebce2asc_tbl[MAX_EBCASCTBL] = {
/* 00   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 10 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 20 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 30 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 40   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      sp                                     */
/*                       ･    <    (    +    | */
		0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xa5,0x3c,0x28,0x2b,0x7c,
/* 50 */
/*      &                                      */
/*             !         $    *    )    ;      */
		0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x21,0x00,0x24,0x2a,0x29,0x3b,0x00,
/* 60 */
/*      -    /                                 */
/*                       ,    %    _    >    ? */
		0x2d,0x2f,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x2c,0x25,0x5f,0x3e,0x3f,
/* 70 */
/*                                             */
/*                  :    #    @    '    =    "  */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x3a,0x23,0x40,0x27,0x3d,0x22,
/* 80   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*           a    b    c    d    e    f    g   */
/*        h    i                               */
		0x00,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
		0x68,0x69,0x00,0x00,0x00,0x00,0x00,0x00,
/* 90 */
/*           j    k    l    m    n    o    p   */
/*        q    r                               */
		0x00,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,
		0x71,0x72,0x00,0x00,0x00,0x00,0x00,0x00,
/* a0 */
/*                 s    t    u    v    w    x  */
/*       y    z                                */
		0x00,0x00,0x73,0x74,0x75,0x76,0x77,0x78,
		0x79,0x7a,0x00,0x00,0x00,0x00,0x00,0x00,
/* b0 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* c0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*           A    B    C    D    E    F    G   */
/*        H    I                               */
		0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
		0x48,0x49,0x00,0x00,0x00,0x00,0x00,0x00,
/* d0 */
/*           J    K    L    M    N    O    P   */
/*        Q    R                               */
		0x00,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,
		0x51,0x52,0x00,0x00,0x00,0x00,0x00,0x00,
/* e0 */
/*      `         S    T    U    V    W    X   */
/*	      Y    Z                               */
		0x60,0x00,0x53,0x54,0x55,0x56,0x57,0x58,
		0x59,0x5a,0x00,0x00,0x00,0x00,0x00,0x00,
/* f0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      0    1    2    3    4    5    6    7   */
/*        8    9                               */
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
		0x38,0x39,0x00,0x00,0x00,0x00,0x00,0x00
	} ;

/*
 * カタカナ変換：ＥＢＣＫ
 */
	static unsigned char ebck2asc_tbl[MAX_EBCASCTBL] = {
/* 00   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 10 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 20 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 30 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 40   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      sp   ｡    ｢    ｣    ､    .    ｦ    ｧ   */
/*        ｨ    ｩ    [    ･    <    (    +    | */
		0x20,0xa1,0xa2,0xa3,0xa4,0x2e,0xa6,0xa7,
		0xa8,0xa9,0x5b,0xa5,0x3c,0x28,0x2b,0x7c,
/* 50   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      &    ｪ    ｫ    ｬ    ｭ    ｮ    ｯ         */
/*                  ]    ¥    *    )    ;       */
		0x26,0xaa,0xab,0xac,0xad,0xae,0xaf,0x00,
		0xb0,0x00,0x5d,0x5c,0x2a,0x29,0x3b,0x00,
/* 60   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      -    /                                 */
/*                  ^    ,    %    _    >    ? */
 		0x2d,0x2f,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x5e,0x2c,0x25,0x5f,0x3e,0x3f,
/* 70   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*                                             */
/*                  :    #    @    '    =    " */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x3a,0x23,0x40,0x27,0x3d,0x22,
/* 80   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*           ｱ    ｲ    ｳ    ｴ    ｵ    ｶ    ｷ   */
/*        ｸ    ｹ    ｺ         ｻ    ｼ    ｽ    ｾ */
		0x00,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xb8,0xb9,0xba,0x00,0xbb,0xbc,0xbd,0xbe,
/* 90   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      ｿ    ﾀ    ﾁ    ﾂ    ﾃ    ﾄ    ﾅ    ﾆ   */
/*        ﾇ    ﾈ    ﾉ              ﾊ    ﾋ    ﾌ */
		0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,
		0xc7,0xc8,0xc9,0x00,0x00,0xca,0xcb,0xcc,
/* a0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*                ﾍ    ﾎ    ﾏ    ﾐ    ﾑ    ﾒ   */
/*        ﾓ    ﾔ    ﾕ         ﾖ    ﾗ    ﾘ    ﾙ */
		0x00,0x00,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,
		0xd3,0xd4,0xd5,0x00,0xd6,0xd7,0xd8,0xd9,
/* b0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*                                             */
/*                  ﾚ    ﾛ    ﾜ    ﾝ    ﾞ    ﾟ */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
/* c0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      {    A    B    C    D    E    F    G   */
/*        H    I                               */
		0x7b,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
		0x48,0x49,0x00,0x00,0x00,0x00,0x00,0x00,
/* d0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      }    J    K    L    M    N    O    P   */
/*        Q    R                               */
		0x7d,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,
		0x51,0x52,0x00,0x00,0x00,0x00,0x00,0x00,
/* e0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      $         S    T    U    V    W    X   */
/*	      Y    Z                               */
		0x24,0x00,0x53,0x54,0x55,0x56,0x57,0x58,
		0x59,0x5a,0x00,0x00,0x00,0x00,0x00,0x00,
/* f0   0,8  1,9  2,a  3,b  4,c  5,d  6,e  7,f */
/*      0    1    2    3    4    5    6    7   */
/*        8    9                               */
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
		0x38,0x39,0x00,0x00,0x00,0x00,0x00,0x00
	} ;

	static unsigned char			asc2ebce_tbl[MAX_EBCASCTBL] ;
	static unsigned char			asc2ebck_tbl[MAX_EBCASCTBL] ;

	static unsigned short			jis2jef_tbl[MAX_JEFJISTBL];
	static unsigned short			jef2jis_tbl[MAX_JEFJISTBL];

/* 2005.08日立対応
 * 英小文字変換：ＥＢＣＨ
 */
	static unsigned char ebch2asc_tbl[MAX_EBCASCTBL] = {
	0x00, 0x01, 0x02, 0x03, 0x80, 0x09, 0x81, 0x7f,		/* 00 */
	0x82, 0x83, 0x84, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x85, 0x0a, 0x08, 0x87,		/* 10 */
	0x18, 0x19, 0x1a, 0x88, 0x1c, 0x1d, 0x1e, 0x1f,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x86, 0x17, 0x1b,		/* 20 */
	0x8e, 0x8f, 0x90, 0x91, 0x92, 0x05, 0x06, 0x07,
	0x93, 0x94, 0x16, 0x95, 0x96, 0x97, 0x98, 0x04,		/* 30 */
	0x99, 0x9a, 0x9b, 0x9c, 0x14, 0x15, 0x9d, 0x9e,
	0x20, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,		/* 40 */
	0xa8, 0xa9, 0x5b, 0x2e, 0x3c, 0x28, 0x2b, 0x21,
	0x26, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xa0,		/* 50 */
	0xb0, 0x61, 0x5d, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
	0x2d, 0x2f, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,		/* 60 */
	0x68, 0x69, 0x7c, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
	0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,		/* 70 */
	0x72, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
	0x73, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,		/* 80 */
	0xb8, 0xb9, 0xba, 0x74, 0xbb, 0xbc, 0xbd, 0xbe,
	0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6,		/* 90 */
	0xc7, 0xc8, 0xc9, 0x75, 0x76, 0xca, 0xcb, 0xcc,
	0x77, 0x7e, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2,		/* a0 */
	0xd3, 0xd4, 0xd5, 0x78, 0xd6, 0xd7, 0xd8, 0xd9,
	0x79, 0x7a, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0x9f,		/* b0 */
	0xe0, 0xe1, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
	0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,		/* c0 */
	0x48, 0x49, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec,
	0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,		/* d0 */
	0x51, 0x52, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2,
	0x5c, 0xf3, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,		/* e0 */
	0x59, 0x5a, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,		/* f0 */
	0x38, 0x39, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
	} ;

	static unsigned char			asc2ebch_tbl[MAX_EBCASCTBL] ;

	static unsigned short			jis2kei_tbl[MAX_KEIJISTBL];
	static unsigned short			kei2jis_tbl[MAX_KEIJISTBL];


/* ----------------------------------------------------------------------------
	関数本体の定義
---------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
	ＡＳＣ−ＥＢＣＤＩＣ変換ＴＢＬを初期化し、メモリに展開する。
---------------------------------------------------------------------------- */
int	ascebc_init()
{
	unsigned char	asce;
	unsigned char	asck;
	unsigned char	asch;

	int				ebc;

	for(ebc = 0; ebc < MAX_EBCASCTBL; ebc++) {
		asce = ebce2asc_tbl[ebc];
		asck = ebck2asc_tbl[ebc];
		asch = ebch2asc_tbl[ebc];

		asc2ebce_tbl[asce] = (unsigned char)ebc;
		asc2ebck_tbl[asck] = (unsigned char)ebc;
		asc2ebch_tbl[asch] = (unsigned char)ebc;
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	JEF-JIS変換TBLを読込、メモリに展開する。
---------------------------------------------------------------------------- */
int	jefjis_init(char *fname)
{
	int				i;
	unsigned short	jef;
	unsigned short	jis;
	unsigned short	tmp;
	FILE			*fp;

	if(sizeof(short) != 2) {
		return -1;
	}

	if( (fp = fopen(fname, "rb")) == NULL)
	{
		fprintf(stderr,"** err ** : cannot open [%s]\n", fname);
		return -1;
	}

	for(i = 0; i < MAX_JEFJISTBL; i++)
	{
		jis = i;
		fread(&tmp, 2, 1, fp);
		jef = xswap( tmp );				/* 上下入替 */
		jis2jef_tbl[jis] = jef;
	}

	for(i = 0; i < MAX_JEFJISTBL; i++)
	{
		jef = i;
		fread(&tmp, 2, 1, fp);
		jis = xswap( tmp );				/* 上下入替 */
		jef2jis_tbl[jef]= jis;
	}
	fclose(fp);
	return 0;
}

/* ----------------------------------------------------------------------------
	KEIS-JIS変換TBL、メモリに展開する。
---------------------------------------------------------------------------- */
int	keijis_init()
{
	int				i;
	unsigned short	kei;
	unsigned short	jis;
	unsigned short	tmp;

	if(sizeof(short) != 2) {
		return -1;
	}

	for(i = 0; i < MAX_KEIJISTBL; i++)
	{
		jis = i;
		if( ((jis & 0xff00) >= 0x2100) &&
		    ((jis & 0x00ff) >= 0x0021) &&
		    ((jis & 0xff00) <= 0x7e00) &&
		    ((jis & 0x00ff) <= 0x007e) )
		{
			jis2kei_tbl[jis] = (jis | 0x8080);
		}

		kei = i;
		if( ((kei & 0xff00) >= 0xa100) &&
		    ((kei & 0x00ff) >= 0x00a1) &&
		    ((kei & 0xff00) <= 0xfe00) &&
		    ((kei & 0x00ff) <= 0x00fe) )
		{
			kei2jis_tbl[kei] = (kei & 0x7f7f);
		}
	}
	return 0;
}


/* ----------------------------------------------------------------------------
	JIS->SJIS変換
---------------------------------------------------------------------------- */
unsigned short _jis2sjis(unsigned short jis)
{
	unsigned short 	sjis;
	unsigned char	ch1, ch2;

	ch1 = (unsigned char)(jis >> 8);
	ch2 = (unsigned char)(jis & 0x00ff);
	if ( (ch1 == 0) || (ch2 == 0) )
		return 0;
	if ( (ch1 >= 0x80) || (ch2 >= 0x7f) )
		return 0;
	if ( ch1 & 1 )
		ch2 += 0x1f;
	else
		ch2 += 0x7d;
	if ( ch2 >= 0x7f )
		ch2++;
	ch1 = (ch1 + 1) / 2;
	if ( ch1 >= 0x30 )
		ch1 += 0xb0;
	else
		ch1 += 0x70;
	sjis = ((unsigned short)(ch1) << 8) | (unsigned short)ch2;
	return sjis;
}

/* ----------------------------------------------------------------------------
	SJIS->JIS変換
---------------------------------------------------------------------------- */
unsigned short	_sjis2jis(unsigned short sjis)
{
	unsigned short	jis;
	unsigned char	ch1, ch2;

	ch1 = (unsigned char)(sjis >> 8);
	ch2 = (unsigned char)(sjis & 0x00ff);
	if ( (ch1 < 0x81) || ((ch1 >= 0xA0) && (ch1 <= 0xDF)) )
		return 0;
	if ( (ch2 < 0x40) || (ch2 > 0xFC) )
		return 0;
	if ( ch1 >= 0xE0 )
		ch1 -= 0xb0;
	else
		ch1 -= 0x70;
	ch1 = ch1 * 2;
	if ( ch2 >= 0x80 )
		ch2--;
	if ( ch2 > 0x9d )
		ch2 -= 0x7d;
	else {
		ch2 -= 0x1f;
		ch1--;
	}
	jis = ((unsigned short)(ch1) << 8) | (unsigned short)ch2;
	return jis;
}

/* ----------------------------------------------------------------------------
	JEF<->JIS, EBC<->ASCII変換テーブルのダンプ
---------------------------------------------------------------------------- */
int	code_dmp(char *arg)
{
	int				i;
	unsigned short	jef, jis, sjis, kei;
	unsigned char	ebc, asc;

	if(strcmp(arg, "jef2sjis") == 0)
	{
		printf("JEF->JIS->SJIS");
		for(i = 0; i < MAX_JEFJISTBL; i++)
		{
			jef = i;
			jis = _jef2jis( jef );
			sjis = _jis2sjis( jis );

			if(jef % 4 == 0) printf("\n");
			printf("%04x:%04x:%04x(", jef, jis , sjis );
			putknj( sjis );
			printf(") ");
		}
		printf("\n");
	}else if(strcmp(arg, "kei2sjis") == 0)
	{
		printf("KEI->JIS->SJIS");
		for(i = 0; i < MAX_KEIJISTBL; i++)
		{
			kei = i;
			jis = _kei2jis( kei );
			sjis = _jis2sjis( jis );

			if(kei % 4 == 0) printf("\n");
			printf("%04x:%04x:%04x(", kei, jis , sjis );
			putknj( sjis );
			printf(") ");
		}
		printf("\n");
	}
	else if(strcmp(arg, "ebce2asc") == 0)
	{
		printf("EBCE->ASC");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			ebc = i;
			asc = _ebce2asc( ebc);
				if(ebc % 8 == 0) printf("\n");
			printf("%02x-%02x(%c):", ebc, asc, asc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "ebck2asc") == 0)
	{
		printf("EBCK->ASC");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			ebc = i;
			asc = _ebck2asc( ebc);
			if(ebc % 8 == 0) printf("\n");
			printf("%02x-%02x(%c):", ebc, asc, asc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "ebce2asc") == 0)
	{
		printf("EBCE->ASC");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			ebc = i;
			asc = _ebce2asc( ebc);
			if(ebc % 8 == 0) printf("\n");
			printf("%02x-%02x(%c):", ebc, asc, asc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "ebch2asc") == 0)
	{
		printf("EBCH->ASC");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			ebc = i;
			asc = _ebch2asc( ebc);
			if(ebc % 8 == 0) printf("\n");
			printf("%02x-%02x(%c):", ebc, asc, asc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "asc2ebck") == 0)
	{
		printf("ASC->EBCK");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			asc = i;
			ebc = _asc2ebck( asc );
			if(asc % 8 == 0) printf("\n");
			printf("(%c)%02x-%02x:", asc, asc, ebc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "asc2ebce") == 0)
	{
		printf("ASC->EBCE");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			asc = i;
			ebc = _asc2ebce( asc );
			if(asc % 8 == 0) printf("\n");
			printf("(%c)%02x-%02x:", asc, asc, ebc);
		}
		printf("\n");
	}
	else if(strcmp(arg, "asc2ebch") == 0)
	{
		printf("ASC->EBCH");
		for(i = 0; i < MAX_EBCASCTBL; i++)
		{
			asc = i;
			ebc = _asc2ebch( asc );
			if(asc % 8 == 0) printf("\n");
			printf("(%c)%02x-%02x:", asc, asc, ebc);
		}
		printf("\n");
	}
	else
	{
		return -1;
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	JEF->SJIS変換(K-in,K-outなし)
---------------------------------------------------------------------------- */
int	jef22sjis(unsigned char *jef, int jef_bytes,
			  char *sjis, int *sjis_bytes)
{
	int				i;
	unsigned short	a_jef;
	unsigned short	a_jis;
	unsigned short	a_sjis;

	*sjis_bytes = jef_bytes ;
	for(i = 0; i < jef_bytes; i++, i++ )
	{
		/* 2Byteづつ変換する． */
		a_jef = ((unsigned short)jef[i] << 8) | (unsigned short)jef[i+1];
		a_jis = _jef2jis( a_jef );
		a_sjis = _jis2sjis( a_jis );
		sjis[i]   = a_sjis >> 8;		/* 上位バイト */
		sjis[i+1] = a_sjis & 0x00ff;	/* 下位バイト */
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	JEF->SJIS変換(K-in,K-outあり)
---------------------------------------------------------------------------- */
int	jef12sjis(unsigned char *jef, int jef_bytes,
			  char *sjis, int *sjis_bytes)
{
	int				i;
	unsigned short	a_jef;
	unsigned short	a_jis;
	unsigned short	a_sjis;

	*sjis_bytes = jef_bytes - 2;
	for(i = 1; i < jef_bytes - 1; i++, i++ )
	{
		/* 2Byteづつ変換する． */
		a_jef = ((unsigned short)jef[i] << 8) | (unsigned short)jef[i+1];
		a_jis = _jef2jis( a_jef );
		a_sjis = _jis2sjis( a_jis );
		sjis[i]   = a_sjis >> 8;		/* 上位バイト */
		sjis[i+1] = a_sjis & 0x00ff;	/* 下位バイト */
	}
	return 0;
}


/* ----------------------------------------------------------------------------
	KEIs->SJIS変換(K-in,K-outなし)
	2005.08 日立対応
---------------------------------------------------------------------------- */
int	kei22sjis(unsigned char *kei, int kei_bytes,
			  char *sjis, int *sjis_bytes)
{
	int				i;
	unsigned short	a_kei;
	unsigned short	a_jis;
	unsigned short	a_sjis;

	*sjis_bytes = kei_bytes ;
	for(i = 0; i < kei_bytes; i++, i++ )
	{
		/* 2Byteづつ変換する． */
		a_kei = ((unsigned short)kei[i] << 8) | (unsigned short)kei[i+1];
		a_jis = _kei2jis( a_kei );
		a_sjis = _jis2sjis( a_jis );
		sjis[i]   = a_sjis >> 8;		/* 上位バイト */
		sjis[i+1] = a_sjis & 0x00ff;	/* 下位バイト */
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	KEIs->SJIS変換(K-in,K-outあり)
	2005.08日立対応
---------------------------------------------------------------------------- */
int	kei12sjis(unsigned char *kei, int kei_bytes,
			  char *sjis, int *sjis_bytes)
{
	int				i;
	unsigned short	a_kei;
	unsigned short	a_jis;
	unsigned short	a_sjis;

	*sjis_bytes = kei_bytes - 4;
	for(i = 2; i < kei_bytes - 2; i++, i++ )
	{
		/* 2Byteづつ変換する． */
		a_kei = ((unsigned short)kei[i] << 8) | (unsigned short)kei[i+1];
		a_jis = _kei2jis( a_kei );
		a_sjis = _jis2sjis( a_jis );
		sjis[i]   = a_sjis >> 8;		/* 上位バイト */
		sjis[i+1] = a_sjis & 0x00ff;	/* 下位バイト */
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	EBCｶﾅ->ASCII変換
---------------------------------------------------------------------------- */
int	ebck2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes)
{
	int				i;

	*ascii_bytes = ebc_bytes;
	for(i = 0; i < ebc_bytes; i++)
	{
		ascii[i] = _ebck2asc( ebc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	EBC英小文字->ASCII変換
---------------------------------------------------------------------------- */
int ebce2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes)
{
	int				i;

	*ascii_bytes = ebc_bytes;
	for(i = 0; i < ebc_bytes; i++)
	{
		ascii[i] = _ebce2asc( ebc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	EBC日立->ASCII変換
---------------------------------------------------------------------------- */
int ebch2asc(unsigned char *ebc, int ebc_bytes,
			 char *ascii, int *ascii_bytes)
{
	int				i;

	*ascii_bytes = ebc_bytes;
	for(i = 0; i < ebc_bytes; i++)
	{
		ascii[i] = _ebch2asc( ebc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	ASCII->EBC英小文字変換
---------------------------------------------------------------------------- */
int asc2ebce(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes)
{
	int				i;

	*ebc_bytes = asc_bytes;
	for(i = 0; i < asc_bytes; i++)
	{
		ebc[i] = _asc2ebce( asc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	ASCII->EBCｶﾅ変換
---------------------------------------------------------------------------- */
int asc2ebck(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes)
{
	int				i;

	*ebc_bytes = asc_bytes;
	for(i = 0; i < asc_bytes; i++)
	{
		ebc[i] = _asc2ebck( asc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	ASCII->EBC日立変換
---------------------------------------------------------------------------- */
int asc2ebch(unsigned char *asc, int asc_bytes,
			 char *ebc, int *ebc_bytes)
{
	int				i;

	*ebc_bytes = asc_bytes;
	for(i = 0; i < asc_bytes; i++)
	{
		ebc[i] = _asc2ebch( asc[i] );
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	BIT->1Byte Character変換(Byte単位に変換)
---------------------------------------------------------------------------- */
int bit2cbit(unsigned char *bit, int bit_bytes,
			 char *charbit, int *charbit_bytes)
{
	static char			bittbl[16][4] =
	{ "0000","0001","0010","0011",
	  "0100","0101","0110","0111",
	  "1000","1001","1010","1011",
	  "1100","1101","1110","1111" };
	int				i;
	unsigned char	one_byte;
	unsigned char	left;
	unsigned char	right;

	*charbit_bytes = bit_bytes * 8;
	for(i = 0; i < bit_bytes; i++){
		one_byte = bit[i];
		left  = one_byte >> 4;
		right = one_byte & 0x0f;

		charbit[i*8]     = bittbl[left][0];
		charbit[i*8 + 1] = bittbl[left][1];
		charbit[i*8 + 2] = bittbl[left][2];
		charbit[i*8 + 3] = bittbl[left][3];

		charbit[i*8 + 4] = bittbl[right][0];
		charbit[i*8 + 5] = bittbl[right][1];
		charbit[i*8 + 6] = bittbl[right][2];
		charbit[i*8 + 7] = bittbl[right][3];
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	HexDump
---------------------------------------------------------------------------- */
int hexdmp(unsigned char *data, int data_bytes,
			 char *dmp, int *dmp_bytes)
{
	int				i;

	*dmp_bytes = data_bytes * 2;
	for(i = 0; i <data_bytes; i++){
		sprintf( dmp + i*2, "%02x", data[i]);
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	HexData
---------------------------------------------------------------------------- */
int hexset(char *hex,  int hex_bytes,
	unsigned char *data, int *data_bytes)
{
	int				data_idx;
	int				hex_idx;
	unsigned char	byte;
	unsigned char	halfu;
	unsigned char	halfl;
	unsigned char	tmph;

	*data_bytes = hex_bytes / 2;
	for(hex_idx = 0; hex_idx < hex_bytes; hex_idx++, hex_idx++){
		tmph = hex[hex_idx];
		if('0' <= tmph && tmph <= '9' ) {
			halfu = tmph - '0';
		}
		else if('a' <= tmph && tmph <= 'f') {
			halfu = tmph - 'a' + 10;
		}
		else if('A' <= tmph && tmph <= 'F') {
			halfu = tmph - 'A' + 10;
		}
		else {
			halfu = 0x00;
		}

		tmph = hex[hex_idx + 1];
		if('0' <= tmph && tmph <= '9' ) {
			halfl = tmph - '0';
		}
		else if('a' <= tmph && tmph <= 'f') {
			halfl = tmph - 'a' + 10;
		}
		else if('A' <= tmph && tmph <= 'F') {
			halfl = tmph - 'A' + 10;
		}
		else {
			halfl = 0x00;
		}

		data_idx = hex_idx / 2;
		byte = (halfu << 4) | (0x0f & halfl);
		data[data_idx] = byte;
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	PACK->ASCII変換.符号は(+=C,-=D)
---------------------------------------------------------------------------- */
int pack2ascii(unsigned char *pack, int pack_bytes,
			   char *ascii, int *ascii_bytes, int machine)
{
	int				sign;
	int				i;
	unsigned char	left;
	unsigned char	right;
	unsigned char	one_byte;

	*ascii_bytes = pack_bytes * 2;

	switch(machine) {
	case FACOM_MSP:
		sign = (pack[pack_bytes - 1] & 0x01) * (-2) + 1;
		break;
	default:
		sign = 1;
		break;
	}

	if(sign > 0) {
		ascii[0] = '+';
	} else {
		ascii[0] = '-';
	}

	for(i = 0; i <  pack_bytes - 1; i++){
		one_byte = (int)pack[i];
		left  = one_byte >> 4;
		right = one_byte & 0x0f;
		ascii[i*2 + 1] = left + '0';
		ascii[i*2 + 2] = right + '0';
	}

	one_byte = (int)pack[pack_bytes - 1];
	left = one_byte >> 4;
	ascii[*ascii_bytes - 1] = left + '0';

	return 0;
}

/* ----------------------------------------------------------------------------
	PACK->4byte integer変換.符号は(+=C,-=D)
---------------------------------------------------------------------------- */
int pack2bin4(unsigned char *pack, int pack_bytes,
				Bin4 *binary, int *binary_bytes, int machine)
{
	int				sign;				/* 符号(C:1, D:-1) */
	int				i;
	Bin4			work;				/* 変換ワーク */
	unsigned char	one_byte;			/* PACK->Binary変換中の１バイト */
	unsigned char	left;				/* 変換中１バイトの左半バイト */
	unsigned char	right;				/* 変換中１バイトの右半バイト */

	*binary_bytes = sizeof(Bin4);
	switch(machine) {
	case FACOM_MSP:
		sign = (pack[pack_bytes - 1] & 0x01) * (-2) + 1;
		break;
	default:
		sign = 1;
		break;
	}

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

	return 0;
}

/* ----------------------------------------------------------------------------
	PACK->8byte integer変換.符号は(+=C,-=D)
		MSVCのみ対応、ほかｺﾝﾊﾟｲﾗへ不明
---------------------------------------------------------------------------- */
int pack2bin8(unsigned char *pack, int pack_bytes,
				Bin8 *binary, int *binary_bytes, int machine)
{
	int				sign;				/* 符号(C:1, D:-1) */
	int				i;
	Bin8			work;				/* 変換ワーク */
	unsigned char	one_byte;			/* PACK->Binary変換中の１バイト */
	unsigned char	left;				/* 変換中１バイトの左半バイト */
	unsigned char	right;				/* 変換中１バイトの右半バイト */

	*binary_bytes = sizeof(Bin8);

	switch(machine) {
	case FACOM_MSP:
		sign = (pack[pack_bytes - 1] & 0x01) * (-2) + 1;
		break;
	default:
		sign = 1;
		break;
	}

	work = 0;
	for(i = 0; i <  pack_bytes - 1; i++){
		one_byte = (int)pack[i];
		left  = one_byte >> 4;
		right = one_byte & 0x0f;
		work = work * 100 + left * 10 + right;	/* ２桁づつ変換 */
	}

	one_byte = (int)pack[pack_bytes - 1];
	left = one_byte >> 4;
	work = work * 10 + left;
	*binary = work * sign;

	return 0;
}


/* ----------------------------------------------------------------------------
	2Byte integer ->ASCII変換.
---------------------------------------------------------------------------- */
int bin22asc(unsigned char *binary, int bin_bytes,
			 char *asc, int *asc_bytes, int machine)
{

	short	tmp;

	switch(machine) {
	case FACOM_MSP:
		tmp = (((short)binary[0])<<8) | binary[1];
		break;
	case INTEL86:
		tmp = (((short)binary[1])<<8) | binary[0];
		break;
	}

	sprintf(asc, "%6d", tmp);
	*asc_bytes = 6;

	return 0;
}

/* ----------------------------------------------------------------------------
	4Byte integer ->ASCII変換.
---------------------------------------------------------------------------- */
int bin42asc(unsigned char *binary, int bin_bytes,
			 char *asc, int *asc_bytes, int machine)
{

	Bin4	tmp;

	switch(machine) {
	case FACOM_MSP:
		tmp = (((Bin4)binary[0])<<24) | (((Bin4)binary[1])<<16) |
			  (((Bin4)binary[2])<<8)  | binary[3];
		break;
	case INTEL86:
		tmp = (((Bin4)binary[3])<<24) | (((Bin4)binary[2])<<16) |
			  (((Bin4)binary[1])<<8)  | binary[0];
		break;
	}

	sprintf(asc, "%11ld", tmp);
	*asc_bytes = 11;
	return 0;
}

/* ----------------------------------------------------------------------------
	8Byte integer ->ASCII変換.
---------------------------------------------------------------------------- */
int bin82asc(unsigned char *binary, int bin_bytes,
			 char *asc, int *asc_bytes, int machine)
{

	Bin8		tmp;

#ifdef	INT64
	switch(machine) {
	case FACOM_MSP:
		tmp = (((Bin8)binary[0])<<56) | (((Bin8)binary[1])<<48) |
			  (((Bin8)binary[2])<<40) | (((Bin8)binary[3])<<32) |
              (((Bin8)binary[4])<<24) | (((Bin8)binary[5])<<16) |
			  (((Bin8)binary[6])<<8)  | binary[7];
		break;
	case INTEL86:
		tmp = (((Bin8)binary[7])<<56) | (((Bin8)binary[6])<<48) |
			  (((Bin8)binary[5])<<40) | (((Bin8)binary[4])<<32) |
              (((Bin8)binary[3])<<24) | (((Bin8)binary[2])<<16) |
			  (((Bin8)binary[1])<<8)  | binary[0];
		break;
	}
	/*
	BUG:_int64を出力できない．
	*/
	sprintf(asc, "%20ld", tmp);

#endif

	*asc_bytes = 20;
	return 0;
}

/* ----------------------------------------------------------------------------
	特定のBIT->1Byte Character変換
---------------------------------------------------------------------------- */
int abit2cbit(unsigned char *byte, int bit,
			  unsigned char *cbit, int *cbit_bytes)
{
	int				base;
	int				offset;
	unsigned char	one_byte;
	static int		bitmask[8] = {
		128,	/* offset = 0 は 1BIT目 */
		64,		/*        = 1 は 2BIT目 */
		32,		/*        = 2 は 3BIT目 */
		16,		/*        = 3 は 4BIT目 */
		8,		/*        = 4 は 5BIT目 */
		4,		/*        = 5 は 6BIT目 */
		2,		/*        = 6 は 7BIT目 */
		1		/*        = 7 は 8BIT目 */
	} ;

	*cbit_bytes = 1;
	base   = bit / 8;	/* bit is 0 origin */
	offset = bit % 8;	/* offset is 0-7   */

	one_byte = byte[base];
	*cbit    = (((one_byte & bitmask[offset]) == 0) ?  0 : 1) + '0';
	return 0;
}

/* ----------------------------------------------------------------------------
	ASCII->PACK変換.(符号付きﾃｷｽﾄ数字を変換する)
---------------------------------------------------------------------------- */
int asc2pack(         char *ascii, int asc_bytes,
			 unsigned char *pack,  int *pack_bytes)
{
	int				num_first;			/* 空白，符号以外（数字）の最初の位置	*/
	int				num_end;			/* 数字の最後の位置						*/
	int				keta;				/* 数値の桁数（符号を含まない）			*/
	int				keta_idx;			/* asciiを走査するIDX					*/
	int				pack_idx;			/* 出力packを走査するIDX				*/
	int				half_idx;			/* 出力を1/2ﾊﾞｲﾄづつ走査するIDX			*/
	unsigned char	a_digit;			/* 入力の数値一桁						*/
	unsigned char	sign;				/* 数値の符号(C:+,D:-)					*/
	double			tmp;
	unsigned char	x;

										/* 0-9が最初に表れる位置				*/
	num_first = strcspn( ascii, "0123456789" );
										/* 0-9のみの部分文字列長：
										   0-9以外の文字が表れる位置			*/
	keta      = strspn( ascii + num_first, "0123456789" );

	tmp = (double)(keta + 1) / 2;
	*pack_bytes = kiriage( tmp );

	if( ascii[num_first - 1] == '-') {
		sign = 0x0d;
	} else {
		sign = 0x0c;
	}

	for(keta_idx = 0; keta_idx < keta; keta_idx++) {
		a_digit = ascii[num_first + keta_idx];

		if(keta % 2 == 0) {
			half_idx = keta_idx + 1;
		} else {
			half_idx = keta_idx;
		}

		pack_idx = half_idx / 2;
		x = pack[pack_idx];
		if(half_idx % 2 == 0) {
										/* 偶数桁は上位半バイト					*/
			pack[pack_idx] = upper_half(x, a_digit);
		} else {
										/* 奇数桁は下位半バイト					*/
			pack[pack_idx] = lower_half(x, a_digit);
		}
	}

	x = pack[*pack_bytes - 1];
	pack[*pack_bytes - 1] = lower_half(x, sign);
	return 0;
}

/* ----------------------------------------------------------------------------
	ASCII->BIT．(ＢＩＴ文字列を変換する)
---------------------------------------------------------------------------- */
int asc2bit8(          char *ascii, int asc_bytes,
			 unsigned char *bit,   int *bit_bytes)
{
	int				bit_first;			/* 「０１」の最初の位置					*/
	int				keta;				/* ＢＩＴ文字列の桁数（８固定）			*/
	int				keta_idx;			/* asciiを走査するIDX					*/
	int				bit_idx;			/* 出力bitを走査するIDX					*/
	unsigned char	a_bit;				/* 入力のＢＩＴ一桁						*/
	unsigned char	c;
	unsigned char	bitmask[] = {
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
	};

										/* 01が最初に表れる位置				*/
	bit_first = strcspn( ascii, "01" );
										/* 01のみの部分文字列長：
										   01以外の文字が表れる位置			*/
	keta      = strspn( ascii + bit_first, "01" );

	if(keta != 8) {
		*bit_bytes = 0;
		return -1;
	}
	*bit_bytes = 1;

	c = 0x00;
	for(keta_idx = 0; keta_idx < keta; keta_idx++) {
		a_bit = ascii[bit_first + keta_idx];
		bit_idx = 7 - keta_idx;

		if(a_bit == '1') {
			c |= bitmask[bit_idx];
		}
	}
	*bit = c;
	return 0;
}
/* ----------------------------------------------------------------------------
	SJS->JEF(K-in,K-outなし)
---------------------------------------------------------------------------- */
int sjis2jef2(unsigned char *sjis, int sjis_bytes,
	          unsigned char *jef,  int *jef_bytes)
{
	int				i;
	unsigned short	a_sjis;
	unsigned short	a_jis;
	unsigned short	a_jef;

	*jef_bytes = sjis_bytes;
	for(i = 0; i < sjis_bytes; i++, i++)
	{
		a_sjis	= ((unsigned short)sjis[i] << 8) | (unsigned short)sjis[i+1];
		a_jis	= _sjis2jis( a_sjis );
		a_jef	= _jis2jef( a_jis );
		jef[i]	= a_jef >> 8;
		jef[i+1] = a_jef & 0x00ff;
	}
	return 0;
}

/* ----------------------------------------------------------------------------
	SJS->KEIs(K-in,K-outなし)
	2005.08日立対応
---------------------------------------------------------------------------- */
int sjis2kei2(unsigned char *sjis, int sjis_bytes,
	          unsigned char *kei,  int *kei_bytes)
{
	int				i;
	unsigned short	a_sjis;
	unsigned short	a_jis;
	unsigned short	a_kei;

	*kei_bytes = sjis_bytes;
	for(i = 0; i < sjis_bytes; i++, i++)
	{
		a_sjis	= ((unsigned short)sjis[i] << 8) | (unsigned short)sjis[i+1];
		a_jis	= _sjis2jis( a_sjis );
		a_kei	= _jis2kei( a_jis );
		kei[i]	= a_kei >> 8;
		kei[i+1] = a_kei & 0x00ff;
	}
	return 0;
}



#else

/* ----------------------------------------------------------------------------
	外部変数の宣言
---------------------------------------------------------------------------- */
	extern unsigned char		ebce2asc_tbl[MAX_EBCASCTBL];
	extern unsigned char		ebck2asc_tbl[MAX_EBCASCTBL];
	extern unsigned char		ebch2asc_tbl[MAX_EBCASCTBL];	/* 2005.08日立対応 */

	extern unsigned char		asc2ebce_tbl[MAX_EBCASCTBL];
	extern unsigned char		asc2ebck_tbl[MAX_EBCASCTBL];
	extern unsigned char		asc2ebch_tbl[MAX_EBCASCTBL];	/* 2005.08日立対応 */

	extern unsigned short		jis2jef_tbl[MAX_JEFJISTBL];
	extern unsigned short		jef2jis_tbl[MAX_JEFJISTBL];
	extern unsigned short		jis2kei_tbl[MAX_KEIJISTBL];
	extern unsigned short		kei2jis_tbl[MAX_KEIJISTBL];

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_CODECONV */
