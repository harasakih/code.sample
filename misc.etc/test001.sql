DROP	TABLE IF EXISTS		Test001;
CREATE	TABLE				Test001 (
	int		integer		,	/* ４バイト整数	*/
	num		numeric(13)	,	/* 数値型			*/
	dec		numeric(13,5),	/* 小数点型			*/

	ch		char(4)		,	/* 固定長文字列	*/
	vch		varchar(20),	/* 可変長文字列	*/
	txt		text		,	/* テキスト（制限なし可変長)	*/

	bin		bytea		,	/* バイナリ		*/

	t1		timestamp	,	/* 日付＋時刻	*/
	dt		date		,	/* 日付（時刻なし)	*/

	bl		boolean		,	/* 論理値		*/
	bt		bit(8)
) ;
