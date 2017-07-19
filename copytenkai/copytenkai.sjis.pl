#!/cygdrive/c/Perl/bin/jperl.exe
#
# Configure for Jperl.exe (jperl5.005_03-990822,Perl 5.005_03 Binary build 522)
#

use	strict;
# use utf8;
use CGI;
# use Encode;

#	binmode(STDOUT, ":utf8");
#	binmode(STDOUT, ":sjis");

# GLOVAL VALS
#  CGI
	my	$gForm = new CGI;

#  WORK
	my	$gCopymember;	# ƒRƒs[‹åƒƒ“ƒo–¼
	my	$gCopylib0;		# ƒRƒs[‹å‚k‚h‚a”‚O
	my	$gCopylib1;		# ƒRƒs[‹å‚k‚h‚a”‚P
	my	$gCopylib2;		# ƒRƒs[‹å‚k‚h‚a”‚Q
	

# main
	&get_cgi_parm();

	# html - header
#	print $gForm->header(-type=>'text/html',
#		-charset=>'Shift_JIS');
#	print $gForm->header(-type=>'text/html',-charset=>'utf-8');
	print $gForm->header(-type=>'text/html',-charset=>'Shift_JIS');

	# html - start
	print $gForm->start_html(-title=>'copytenkai result');

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
	print "“ú–{Œê•\Ž¦‚Í‚Ç‚¤‚©‚È\n";
	print '</PRE>';
	print $gForm->hr;

	print '<PRE><FONT FACE="‚l‚r ƒSƒVƒbƒN">';
	
#	my	$infile = '/mnt/d/fedora-httpd-conf/utf8.txt';
	my	$infile = '/mnt/d/fedora-httpd-conf/sjis.txt';
#	my	$infile = 'd://fedora-httpd-conf/Apache‚ÅCGI‚ðŽg‚¦‚é‚æ‚¤‚É‚·‚é.txt';
#	open(IN, '<:encoding(shiftjis)', $infile) || die "$infile\n";
	open(IN, $infile) || die "$infile\n";

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


