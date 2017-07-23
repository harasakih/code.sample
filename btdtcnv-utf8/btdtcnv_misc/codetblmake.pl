#!perl.exe
#
#
#
	use	strict;

	my	($jis, $euc, $sjis);
	my	($ku, $ten);
	my	($ku_start, $ten_start);
	my	$jis_ku_start	=	0x21;
	my	$jis_ten_start	=	0x21;
	my	$euc_ku_start	=	0xa1;
	my	$euc_ten_start	=	0xa1;
	
	open(OT, ">$ARGV[0]")	||	die;
	
	$ku_start	=	$jis_ku_start;
	$ten_start	=	$jis_ten_start;

	for($ku = 0; $ku < 94; $ku++)
	{
		for($ten = 0; $ten < 94; $ten++)
		{
			$jis	= ($ku_start + $ku)*256 + ($ten_start + $ten);
			$euc	=	&jis2euc($jis);		# euc
			$sjis	=	&jis2sjis($jis);	# sjis;
			
			printf (OT "0x%04x,0x%04x\n", $euc, $euc);
		}
	}
	close(OT);
	exit 0;

sub	euc2jis {
	my	($euc)	=	@_;
	
	return	($euc & 0x7f7f);
}

sub	jis2euc {
	my	($jis)	=	@_;
	
	return	($jis | 0x8080);
}


sub	jis2sjis {
	my	($jis)	=	@_;
	my	($upper, $lower);
	
	$upper	=	int($jis / 256);
	$lower	=	$jis % 256;
	
	if( ($upper % 2) eq 0 )
	{
		$upper	=	int($upper / 2) + 0x70;
		$lower	+=	0x7d;
	}
	else
	{
		$upper	=	int(($upper + 1) / 2) + 0x70;
		$lower	+=	0x1f;
	}
	
	if($upper >= 0xa0)
	{
		$upper	+=	0x40;
	}
	
	if($lower >= 0x7f)
	{
		$lower	+=	1;
	}
	
	return ($upper*256 + $lower);
}


sub	sjis2jis {
	my	($sjis)	=	@_;
	my	($upper, $lower);
	
	$upper	=	int($sjis / 256);
	$lower	=	$sjis % 256;
	
	if($upper >= 0xe0)
	{
		$upper	-=	0xb0;
	}
	else
	{
		$upper	-=	0x70;
	}
	$upper	=	$upper * 2;
	
	if($lower >= 0x80)
	{
		$lower	-=	1;
	}
	
	if($lower > 0x9d)
	{
		$lower	-=	0x7d;
	}
	else
	{
		$lower	-=	0x1f;
		$upper	-=	1;
	}
	
	return ($upper*256 + $lower);
}
