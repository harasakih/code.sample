#
#   入力ファイル１で指定された文字列を含むファイルを検索する
#
#   入力１：検索対象文字列一覧
#       ホワイトスーペース区切りの第１項目を検索
#   入力２：検索対象ディレクトリ
#       指定ディレクトリ配下を検索
#   出力１：ファイル名、行番号、見つかった行をそのまま出力
#
# X'28'
# X'29'
# X'38'
# MODE

print "検索対象文字列一覧 :";
chop( $srch_file = <STDIN> );
open( SRCH, $srch_file ) || die ;

print "検索対象ディレクトリ :";
chop( $dir1 = <STDIN> );
opendir(DIR1, $dir1) || die ;

print "検索結果 :";
chop( $out_file = <STDIN> );
open( OUT, ">$out_file" ) || die ;

# 検索対象文字列一覧を内部テーブルに
$i = 0;
print OUT "*** ﾃﾞｨﾚｸﾄﾘ[$dir1]で以下の文字列を検索 ***¥n";
while( <SRCH> ) {
    split(/¥s+/);
    $srch[ $i ] = $_[0];
    print OUT "$srch[ $i ]¥n";
    $i++;
}
$srch_cnt = $i;


# ﾃﾞｨﾚｸﾄﾘ単位のﾙｰﾌﾟ
print OUT "*** 検索結果 ***¥n";
LBL_DIR:
while($in_fname = readdir(DIR1)) {
    $fname = "$dir1¥/$in_fname";
    next LBL_DIR if -d $fname;

# ﾌｧｲﾙ単位の処理
    print "$fname¥n";
    &srch_in_file();
}

closedir(DIR1);
close(SRCH);
close(OUT);


sub srch_in_file {
    open(IN, $fname) || die;
    $l = 0;
    while( <IN> ) {
        $l++;
        $match = 0;
        for($i = 0; $i < $srch_cnt; $i++) {
            if( $_ =‾ /$srch[$i]/) { $match = 1; }
        }
        ($match == 0) || print OUT "$fname($l):$_";
    }
    close(IN);
}


1; # RETURN TRUE
