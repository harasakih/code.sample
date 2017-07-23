#!jperl.exe
#
#

unshift( @INC, 'c:\@users\p3batwww');
$i	=	0;
foreach $x ( @INC ) {
	print	"$i $x\n";
	$i	+=	1;
}

