#!/usr/bin/perl	
#
# 出力をすべてＵＴＦ８にすると、表示の文字化けもない
# もちろん、このファイル(perl)自体もｕｔｆ８
#

use	strict;
use utf8;
use CGI;
use Encode;

	binmode(STDOUT, ":utf8");		# stdoutはutf8にエンドードする
#	binmode(STDOUT, ":sjis");		# sjisで出力してはダメ

# GLOVAL VALS
#  CGI
	my	$gForm = new CGI;

#  WORK
	my	$gCopymember;	# コピー句メンバ名
	my	$gCopylib0;		# コピー句ＬＩＢ＃０
	my	$gCopylib1;		# コピー句ＬＩＢ＃１
	my	$gCopylib2;		# コピー句ＬＩＢ＃２
	

# main
	&get_cgi_parm();

	# html - header
	print $gForm->header(-type    =>'text/html',
                         -charset =>'utf-8');

	# html - start
	print $gForm->start_html(-title   =>'copytenkai result',
                             -charset =>'utf-8',
                             -encoding=>'utf-8',
                             -lang    =>'ja-JP');

	# html - body
	print $gForm->strong("COPYBOOK $gCopymember ");
	print $gForm->hr;
	print '<PRE>';
	print "here is in PRE\n";
	print "is thie ok ??\n";
	print "Copymember = $gCopymember\n";
	print "Copylib0   = $gCopylib0\n";
	print "Copylib1   = $gCopylib1\n";
	print "Copylib2   = $gCopylib2\n";
	print "日本語表示はどうかな\n";
	print '</PRE>';
	print $gForm->hr;

	print '<PRE><FONT FACE="ＭＳ ゴシック">';
	
# sjisのファイルは、入力のエンコードを明示的にｓｊｉｓ
	my	$infile = '/mnt/d/fedora-httpd-conf/sjis.txt';
	open(IN, '<:encoding(shiftjis)', $infile) || die "$infile\n";

#	my	$infile = '/mnt/d/fedora-httpd-conf/utf8.txt';
#	open(IN, '<:encoding(utf8)', $infile) || die "$infile\n";


	
	while( <IN> ) {
		print;
	}

	close(IN);
	print '</FONT></PRE>';

	# html - end
	print $gForm->end_html;
	exit;



sub		get_cgi_parm {
	
	$gCopymember	= $gForm->param('copymember');
	$gCopylib0		= $gForm->param('copylib0');
	$gCopylib1		= $gForm->param('copylib1');
	$gCopylib2		= $gForm->param('copylib2');

}


