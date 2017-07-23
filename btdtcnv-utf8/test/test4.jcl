#
# btdtcnvの動作定義
#               codeconv        unpack          fmtdmp
#               コード変換      文字化          項目ダンプ
#       -----------------------------------------------------------------------
#       CH      半角変換        半角変換        半角変換
#       PD      コピー          UNPACK          UNPACK
#       ZD      コピー          ＺＤ文字変換    ＺＤ文字変換
#       NC      全角変換        全角変換        全角変換
#       B8      コピー          ビットＣＨＡＲ  ビットＣＨＡＲ
#       HX      コピー          ＨＥＸダンプ    ＨＥＸダンプ
#       CP      コピー          コピー          不可
#

[btdtcnv]
# execute=codeconv | unpack | fmtdmp
# maxfmtprint : fmtdmp指定時の出力最大桁数（０は１レコード＝１行）
# maxicount   : 出力最大レコード数（０は、全部）
execute=unpack
maxfmtprint=200
maxicount=0

[infile]
recfm=F
lrecl=16
code_han=EBC
code_zen=KEIS83
vbtype=host

[outfile]
# delimiter=# は区切りなし
# cr=0 (no-crlf) / cr=1(cr) / cr=2(crlf)
#
recfm=T
#lrecl=32
code_han=ASC
code_zen=SJIS
delimiter=,
cr=2

[mapfile]
#マップファイルの開始位置をシフト
adjust=0

[codetbl]
# 半角、全角の変換指示テーブル（未指定文字は、replace_han,replace_zenで置換え）
#
tbl_han=..\hantbl.csv
tbl_zen=..\zentbl.csv
#
# 規定文字以外のアベンド有無と置換え文字
#   ASCSP=0x20    SJISSP=0x8140
#
abend_han=0
replace_han=0x40
abend_zen=0
replace_zen=0x8140
