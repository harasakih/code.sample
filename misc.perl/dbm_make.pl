#!jperl
#
#	DBMMAKE sample
#

print	"File to convert to DBM : ";
chop( $f_in = <STDIN> );
open(IN, $f_in) || die "Input file erroe [$f_in]";

print	"DBM file : ";
chop( $f_dbm = <STDIN> );
dbmopen(%db, $f_dbm, 0666) || die "DBM file error [$f_dbm]";

while( <IN> ) {
	($lib, $module, $prgmei, $lang, $address, $grp, $owner) = split(/,/);
	$module	=~ s/\s+//;
	$lib	=~ s/\s+//;
	$grp	=~ s/\s+//;

	print "$module\t$prgmei\n";
	$db{ "$lib,$module" } = "$prgmei,$grp";
}

dbmclose(%db);
close(IN);

