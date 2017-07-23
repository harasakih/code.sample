#!jperl
#
#	DBMTEST sample
#

print	"DBM file : ";
chop( $f_dbm = <STDIN> );
dbmopen(%db, $f_dbm, 0666) || die "DBM file error [$f_dbm]";

while( 1 ) {
	print "Library : ";
	chop( $lib = <STDIN> );

	print "Module  : ";
	chop( $module = <STDIN> );

	$tmp = $db{ "$lib,$module" };
	($prgmei, $grp) = split(/,/, $tmp);

	print "Prg:$prgmei,$grp\n";
}

dbmclose(%db);

