#!perl
#<NAME>
#	ftp1.pl		ディレクトリ配下のファイルをftpでupldする
#<SYNOPSIS>
#	perl -s ftp1.pl INDIR LOGFILE
#<AUTOHR>
#	HARASAKI Hiroshi
#<DESCRIPTION>
#	指定ディレクトリ配下の全ファイルをｆｔｐする
#<OPTION>
#<VERSION>
#	Ver0.0-0.0	2006/02/26	初期作成
#<BUGS>
#	

#
use strict;
use Net::FTP;

my	($gIn_Dir,   $gOt_Dir,   $gLg_dir);
my	($gIn_Fname, $gOt_Fname, $gLg_Fname);
my	($gIn_Path,  $gOt_Path,  $gLg_Path);

	&main;
	
sub	main {
	my	($host, $user, $pass);
	my	($local_file, $remote_file);
	
	print	"\n\n指定ディレクトリ配下の全ファイルをｆｔｐする\n";
	if( $ARGV[0] eq "" ) {
		print	"ｆｔｐ対象ディレクトリ :";
		chop($gIn_Dir 	= <STDIN>);
	} else {
		$gIn_Dir 		= $ARGV[0];
	}
	opendir(INDIR, $gIn_Dir) || die "Directory error $gIn_Dir";

	if( $ARGV[1] eq "" ) {
		print	"ＬＯＧファイル :";
		chop($gLg_Path	= <STDIN>);
	} else {
		$gLg_Path		= $ARGV[1];
	}
	open(LG, ">$gLg_Path") || die "File error $gLg_Path";

#	host-name
	print "ftp-Host: "; 		chop($host = <STDIN>);
	if($host eq '')				{ $host = 'localhost'; }
#	user-name
	print "ftp-User: "; 		chop($user = <STDIN>);
	if($user eq '')				{ $user = 'ftp'; }
#	user-password
	print "ftp-Password: "; 	chop($pass = <STDIN>);
	if($pass eq '')				{ $pass = 'ftp'; }
#	remode-dir
	print "ftp-RemoteDir: "; 	chop($gOt_Dir = <STDIN>);
	if($gOt_Dir eq '')			{ $gOt_Dir = './'; }

	my $ftp = Net::FTP->new($host)	||	die "can not connection: $@";	# Connect
	$ftp->login($user, $pass) 		||	die $ftp->message;				# ログイン
	$ftp->ascii						||	die $ftp->message;				# アスキーモードに変更
	print "pwd is " . $ftp->pwd() . "\n";


LBL_DIR:
	while($gIn_Fname = readdir(INDIR)) {
		$gIn_Path	=	"$gIn_Dir\/$gIn_Fname";
		next LBL_DIR if -d $gIn_Path;

		$gOt_Fname	=	$gIn_Fname;
		$gOt_Path	=	"$gOt_Dir\/$gOt_Fname";

		print    "$gIn_Path -> $gOt_Path : ";
		print LG "$gIn_Path -> $gOt_Path : ";
		if( $ftp->put($gIn_Path, $gOt_Path) )							# ファイル転送
		{
			print    "success\n";
			print LG "success\n";
		}
		else
		{
			print    "failed\n";
			print LG "failed\n";
			warn $ftp->message;
		}
	}
	closedir(DIR1);
	$ftp->quit;															# 終了

}
