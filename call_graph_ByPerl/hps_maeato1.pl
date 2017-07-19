#!perl.exe
# <NAME>
#	hps_maeato1.pl
# <AGENDA>
#	先行後続リストを入力に、ＣＳＶ化
#	KOZOKU,net,net_next
#	SENKOU,net,net_pre
#

use	strict;
my	($in_dir,   $ot_dir, $gOt_Dbg);
my	($in_fname, $ot_fname);
my	($in_path,  $ot_path );
my	($sbt, $i, $net1);

print	"¥n¥n先行後続リストのＣＳＶ化¥n";
if( $ARGV[0] eq "" ) {
	print	"先行後続リスト(入力) : ";
	chop($in_path = <STDIN>);
} else {
	$in_path = $ARGV[0];
}
open(IN, "$in_path")	|| die "Input file error $in_path¥n";

if( $ARGV[1] eq "" ) {
	print	"出力ファイル : ";
	chop($ot_path = <STDIN>);
} else {
	$ot_path	= $ARGV[1];
}
open(OT, ">$ot_path")	|| die "Output file error $ot_path¥n";


	while( <IN> )
	{
		if( substr($_, 35, 23) eq 'HOPSS ｺｳｿﾞｸ ﾈﾂﾄ ｲﾁﾗﾝﾋﾖｳ' )
		{
			$sbt	=	"kozoku";
		}
		elsif( substr($_, 37, 27) eq 'HOPSS ｾﾝｺｳ ﾈﾂﾄ ｼﾞﾖｳｷﾖｳ ｲﾁﾗﾝ' )
		{
			$sbt	=	"senkou";
		}
		next if substr($_, 1, 3) eq 'ﾈﾂﾄ' ;
		next if substr($_, 1, 10) eq 'B*STA-ﾁｷﾞﾝ';
		next if $sbt !‾ /kozoku|senkou/;
		
		split(/[ ¥n]+/, $_);
		
		if( $_[0] ne '' )
		{
			$net1 = substr($_[0], 1);
			for($i = 1; 1; $i++) 
			{
				last if $_[$i] eq '';		# exit-loop
				print OT "$sbt,$net1,$_[$i]¥n";
			}
		}
	}

close(IN);
close(OT);

