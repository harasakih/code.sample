/*
 ***********************************************************************
 *	$Id: cmdline.h,v 1.2 2006/06/25 13:25:18 hiroshi Exp $
 ***********************************************************************
 *	$Log: cmdline.h,v $
 *	Revision 1.2  2006/06/25 13:25:18  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 * ﾌｧｲﾙ	コマンドラインを処理するためのヘッダーファイル
 */
 
#ifndef		_INC_CMDLINE
#define		_INC_CMDLINE

/* ----------------------------------------------------------------------------
	ＩＮＣＬＵＤＥファイル
---------------------------------------------------------------------------- */
#include	<stdio.h>

/* ----------------------------------------------------------------------------
	構造体の定義
---------------------------------------------------------------------------- */
struct CmdLine {
	char	character;				/* ｵﾌﾟｼｮﾝの指定文字(a-zA-Z)		*/
	char	*argment;				/* 指定文字に後続する引数へのﾎﾟｲﾝﾀ */
	int		opt_hissu;				/* ｵﾌﾟｼｮﾝが必須(1)か任意か		*/
	int		oarg_hissu;				/* ｵﾌﾟｼｮﾝに続くargmentが必須か	*/
	int		opt_sitei;				/* ｵﾌﾟｼｮﾝが指定されたか			*/
	int		oarg_sitei;				/* ｵﾌﾟｼｮﾝに続くargmentが指定されたか */
} ;

/* ----------------------------------------------------------------------------
	関数のプロトタイプ宣言
---------------------------------------------------------------------------- */
int		opt_init(int argc, char *argv[], struct CmdLine opt[]);
int		opt_hissu (struct CmdLine opt[]);
int		oarg_hissu(struct CmdLine opt[]);

#ifdef	MAIN

/* ----------------------------------------------------------------------------
	外部変数の定義
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
	関数の定義
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
処理	:ｺﾏﾝﾄﾞﾗｲﾝ引数をﾁｪｯｸ用内部ﾃｰﾌﾞﾙと突合し、指定されたか否かのFLGと
		 ｵﾌﾟｼｮﾝ毎に引数へのﾎﾟｲﾝﾀをｾｯﾄする。
		 ｴﾗｰの時は、ｴﾗｰを発見した argcを返却。
戻り値	:ｴﾗｰを発見した、ｺﾏﾝﾄﾞﾗｲﾝ引数のargc。正常は0。
		 argv[0]はﾁｪｯｸしないので、正常は0
注意	:突合用内部ﾃｰﾌﾞﾙは struct CmdLine opt[]
		 最後の配列要素は,options[].character == (char)NULL のこと
---------------------------------------------------------------------------- */
int	opt_init(int argc, char *argv[], struct CmdLine opt[])
{
	static	int	ret;
	int		i,j;
	char	a_option;
	
	for(i = 1; i < argc; i++) {
		a_option = argv[i][1];
		/* CMD引数を内部ﾃｰﾌﾞﾙ opt[] と突合ﾁｪｯｸする */
		for(j = 0; opt[j].character != (char)NULL; j++) {
										/* 内部ﾃｰﾌﾞﾙにﾋｯﾄしたら次の引数 */
			if(a_option == opt[j].character) {
				opt[j].opt_sitei = 1;
				opt[j].argment = &argv[i][2];
				if(*opt[j].argment != 0x00) {
										/* opt[j].argment != (char)NULL はNG */
					opt[j].oarg_sitei = 1;
				} else {
					opt[j].oarg_sitei = 0;
				}
				break;
			}
		}
										/* ﾁｪｯｸ用内部ﾃｰﾌﾞﾙにﾋｯﾄしなかったら、ｴﾗｰ */
		if(opt[j].character == (char)NULL) {
			return (ret = i);
		} 
	}
	return 0;
}

/* ----------------------------------------------------------------------------
処理	必須オプションのチェック
---------------------------------------------------------------------------- */
int	opt_hissu(struct CmdLine opt[])
{
	static	int	ret;
	int		i;

	for(i = 0; opt[i].character != (char)NULL; i++) {
		if(opt[i].opt_hissu && !opt[i].opt_sitei) { 
			return (ret = i);
		}
	}
	return 0;
}


/* ----------------------------------------------------------------------------
処理	オプションに続くargmentの必須チェック
---------------------------------------------------------------------------- */
int	oarg_hissu(struct CmdLine opt[])
{
	static	int	ret;
	int		i;

	for(i = 0; opt[i].character != (char)NULL; i++) {
		if(opt[i].opt_sitei != 0) {
			if(opt[i].oarg_hissu && !opt[i].oarg_sitei) { 
				return (ret = i);
			}
		}
	}
	return 0;
}

#else

/* ----------------------------------------------------------------------------
	外部変数の宣言
---------------------------------------------------------------------------- */

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_CMDLINE */
