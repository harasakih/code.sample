#!/usr/bin/perl
#
#

use		strict;
use		File::Tail;

my	($file);
my	($tail_fname);
my	($line);
#
#	COLOR
#
my	$Black		= "\033\[0;30m" ;
my	$Blue		= "\033\[0;34m" ;
my	$Green		= "\033\[0;32m" ;
my	$Cyan		= "\033\[0;36m" ;
my	$Red		= "\033\[0;31m" ;
my	$Purple		= "\033\[0;35m" ;
my	$Brown		= "\033\[0;33m" ;
my	$LGray		= "\033\[0;37m" ;

my	$DGray		= "\033\[1;30m" ;
my	$LBlue		= "\033\[1;34m" ;
my	$LGreen		= "\033\[1;32m" ;
my	$LCyan		= "\033\[1;36m" ;
my	$LRed		= "\033\[1;31m" ;
my	$LPurple	= "\033\[1;35m" ;
my	$Yellow		= "\033\[1;33m" ;
my	$White		= "\033\[1;37m" ;

my	$Bell		= "\a";
	
	
	$tail_fname	=	"/var/log/messages";
	$file=File::Tail->new( name=>$tail_fname, maxinterval=>1, interval=>1 );

	print "\n [/var/log/messages] monitor started !!\n";

	while (defined($line=$file->read))
	{
		if($line =~	/ABEND/) {
			print "$Bell";
			print "$LRed$line";
			print "$Black";
		} elsif($line =~ /WARN/) {
			print "$Green$line";
			print "$Black";
		} elsif($line =~ /END/) {
			print "$Black$line";
			print "$Black";
			next;
		}
	}

END {
	print "$Black Bye !!";
}
