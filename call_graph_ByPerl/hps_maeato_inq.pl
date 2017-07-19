#!perl.exe
# <NAME>
#	hps_maeato_inq.pl
# <AGENDA>
#	先行後続リスト(CSV)を入力に後続ネットを照会
#

use strict;
my ( $in_dir, $ot_dir, $gOt_Dbg );
my ( $in_fname, $ot_fname );
my ( $in_path,  $ot_path );
my ( $cmd_path, $gInput );
my ( %gKozoku,  %gSenkou );
my $gLimit = 20;    # 検索ネストの最大深さ
my %gStack;

&main;
exit 0;

sub main {
    my ( $cmd, $i, $name );

    print "\n\n先行後続リストの照会\n";
    if ( $ARGV[0] eq "" ) {
        print "先行後続リスト(入力) : ";
        chop( $in_path = <STDIN> );
    }
    else {
        $in_path = $ARGV[0];
    }
    open( IN, "$in_path" ) || die "Input file error $in_path\n";

    if ( $ARGV[1] eq "" ) {
        print "出力ファイル : ";
        chop( $ot_path = <STDIN> );
    }
    else {
        $ot_path = $ARGV[1];
    }
    open( OT, ">$ot_path" ) || die "Output file error $ot_path\n";

    if ( $ARGV[2] eq "" ) {
        $gInput = "STDIN";
    }
    else {
        $gInput   = "FILE";
        $cmd_path = $ARGV[2];
        open( CMD, "$cmd_path" ) || die "CMD file error $cmd_path\n";
    }

    &read_csv;

    if ( $gInput eq 'STDIN' ) {
        while (1) {
            print "[SK] name / Q)uit: ";
            chop( $cmd = <STDIN> );
            last if $cmd eq 'Q';
            ( $cmd, $name ) = split( /[ \t\n]+/, $cmd );

            if ( $cmd eq 'K' ) {
                print "----- KOZOKU-INQ [$name] -----\n";
                print OT "----- KOZOKU-INQ [$name] -----\n";
                undef %gStack;
                &kozoku_inq( 0, $name );
            }
            elsif ( $cmd eq 'S' ) {
                print "----- SENKOU-INQ [$name] -----\n";
                print OT "----- SENKOU-INQ [$name] -----\n";
                undef %gStack;
                &senkou_inq( 0, $name );
            }
            else {
                next;
            }
        }
    }
    elsif ( $gInput eq 'FILE' ) {
        while (<CMD>) {
            chop( $cmd = $_ );
            last if $cmd eq 'Q';
            ( $cmd, $name ) = split( /[ \t\n]+/, $cmd );

            if ( $cmd eq 'K' ) {
                print OT "----- KOZOKU-INQ [$name] -----\n";
                undef %gStack;
                &kozoku_inq( 0, $name );
            }
            elsif ( $cmd eq 'S' ) {
                print OT "----- SENKOU-INQ [$name] -----\n";
                undef %gStack;
                &senkou_inq( 0, $name );
            }
            else {
                next;
            }
        }
    }
    else {
        ;
    }

    close(IN);
    close(OT);

    return 0;
}

sub read_csv {
    my ( $net1, $net2, $sbt );

    while (<IN>) {
        ( $sbt, $net1, $net2 ) = split( /[ ,\n]+/, $_ );

        if ( $net1 eq '' ) {
            print OT "net1 is null [$net2]\n";
        }

        if ( $net2 eq '' ) {
            print OT "net2 is null [$net1]\n";
        }

        #		print "$sbt:$net1:$net2:\n";
        if ( $sbt eq 'senkou' ) {
            $gSenkou{$net1} .= "$net2;";
        }
        elsif ( $sbt eq 'kozoku' ) {
            $gKozoku{$net1} .= "$net2;";
        }
        else {
            ;
        }
    }
}

sub kozoku_inq {
    my ( $level, $net ) = @_;
    my ($i);
    my ( $level2, $net2 );

	if($net =‾ /^[ ]*$/) {
		print "net is empty\n";
		exit -1;
	}

# 一定深さ以上は検索を中止する
	if($level > $gLimit) {
		if($gInput eq 'STDIN') { print    "ERROR : Recursive-Search is stopped\n"; }
								 print OT "ERROR : Recursive-Search is stopped\n";
		$gStack{ $net } = 1;
		return -1;
	}



# レベルにあわせて出力開始位置を調整
	for($i = 0; $i < $level; $i++)
	{
		if($gInput eq 'STDIN') { print   " "; }
						 		 print OT " ";
	}

# 検索済の場合は、中止する
	if(defined $gStack{ $net })
	{
		if($gInput eq 'STDIN') { print    "$net : Already appeared\n"; }
								 print OT "$net : Already appeared\n";
		return -1;
	}
	else
	{
		$gStack{ $net } = 1;
		if($gInput eq 'STDIN') { print    "$net\n"; }
								 print OT "$net\n";
	}

# 配下のネットを再帰検索する
	split(/;/, $gKozoku{ $net });
	$level2	=	$level + 1;

	foreach $net2(@_)
	{
		&kozoku_inq( $level2, $net2);
	}
	return 0;
}


sub	senkou_inq {
	my	($level, $net)	=	@_;
	my	($i);
	my	($level2, $net2);

	if($net =‾ /^[ ]*$/) {
		print "net is empty\n";
		exit -1;
	}

# 一定深さ以上は検索を中止する
	if($level > $gLimit) {
		if($gInput eq 'STDIN') { print    "ERROR : Recursive-Search is stopped\n"; }
								 print OT "ERROR : Recursive-Search is stopped\n";
		$gStack{ $net } = 1;
		return -1;
	}


# レベルにあわせて出力開始位置を調整
	for($i = 0; $i < $level; $i++)
	{
		if($gInput eq 'STDIN') { print    " "; }
								 print OT " ";
	}

# 検索済の場合は、中止する
	if(defined $gStack{ $net })
	{
		if($gInput eq 'STDIN') { print    "$net : Already appeared\n"; }
								 print OT "$net : Already appeared\n";
		return -1;
	}
	else
	{
		$gStack{ $net } = 1;
		if($gInput eq 'STDIN') { print    "$net\n"; }
								 print OT "$net\n";
	}

# 配下のネットを再帰検索する
	split(/;/, $gSenkou{ $net });
	$level2	=	$level + 1;

	foreach $net2(@_)
	{
		&senkou_inq( $level2, $net2);
	}
	return 0;
}
