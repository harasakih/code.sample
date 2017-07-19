#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#define		MAX_JIS			65536

//
//	jis2euc
//
void	jis2euc_12(void)
{
	int 		jis;
	int 		euc;

	printf("# JIS,EUC\n");
	for(jis = 0; jis < MAX_JIS; jis++)
	{
		if( ((jis & 0xff00) >= 0x2100) &&
		    ((jis & 0x00ff) >= 0x0021) &&
		    ((jis & 0xff00) <= 0x7e00) &&
		    ((jis & 0x00ff) <= 0x007e) )
		{
			euc	=	(jis | 0x8080);
			printf("0x%2x,0x%2x\n", jis, euc);
		}
	}
}

//
//	euc2jis
//
void	euc2jis_12(void)
{
	int 		jis;
	int 		euc;

	printf("# EUC,JIS\n");
	for(euc = 0; euc < MAX_JIS; euc++)
	{
		if( ((euc & 0xff00) >= 0xa100) &&
		    ((euc & 0x00ff) >= 0x00a1) &&
		    ((euc & 0xff00) <= 0xfe00) &&
		    ((euc & 0x00ff) <= 0x00fe) )
		{
			jis = (euc & 0x7f7f);
			printf("0x%2x,0x%2x\n", euc,jis);
		}
	}
}

//
//	jis2sjis
//
int		_jis2sjis(int jis)
{
	unsigned char		ch1, ch2;
	
		ch1 = (unsigned char)(jis >> 8);
		ch2 = (unsigned char)(jis & 0x00ff);
		if ( (ch1 == 0) || (ch2 == 0) )			return 0;
		if ( (ch1 >= 0x80) || (ch2 >= 0x7f) )	return 0;

		if ( ch1 & 1 ) {
			ch2 += 0x1f;
		} else {
			ch2 += 0x7d;
		}
			
		if ( ch2 >= 0x7f ) ch2++;
		ch1 = (ch1 + 1) / 2;

		if ( ch1 >= 0x30 ) {
			ch1 += 0xb0;
		} else {
			ch1 += 0x70;
		}
		return ((unsigned short)(ch1) << 8) | (unsigned short)ch2;
}
void	jis2sjis_12(void)
{
	int 			jis;
	int 			sjis;

	printf("# JIS,SJIS\n");
	for(jis = 0; jis < MAX_JIS; jis++)
	{
		if( ((jis & 0xff00) >= 0x2100) &&
		    ((jis & 0x00ff) >= 0x0021) &&
		    ((jis & 0xff00) <= 0x7e00) &&
		    ((jis & 0x00ff) <= 0x007e) )
		{
			printf("0x%2x,0x%2x\n", jis, _jis2sjis( jis ));
		}
	}
}

//
//	sjis2jis
//
int		_sjis2jis( int sjis )
{
	unsigned char	ch1, ch2;

	ch1 = (unsigned char)(sjis >> 8);
	ch2 = (unsigned char)(sjis & 0x00ff);
	if ( (ch1 < 0x81) || ((ch1 >= 0xA0) && (ch1 <= 0xDF)) ) return 0;
	if ( (ch2 < 0x40) || (ch2 > 0xFC) ) 					return 0;

	if ( ch1 >= 0xe0 ) {
		ch1 -= 0xb0;
	} else {
		ch1 -= 0x70;
	}

	ch1 = ch1 * 2;
	if ( ch2 >= 0x80 ) ch2--;
	if ( ch2 >  0x9d ) {
		ch2 -= 0x7d;
	} else {
		ch2 -= 0x1f;
		ch1--;
	}
		
	return ((unsigned short)(ch1) << 8) | (unsigned short)ch2;
}
void	sjis2jis_12(void)
{
	int 		jis;
	int 		sjis;

	printf("# SJIS,JIS\n");
	for(sjis = 0; sjis < MAX_JIS; sjis++)
	{
		printf("0x%2x,0x%2x\n", sjis, _sjis2jis( sjis ));
	}
}

void	euc2sjis_12(void)
{
	int 		jis;
	int 		euc;

	printf("# EUC,SJIS\n");
	for(euc = 0; euc < MAX_JIS; euc++)
	{
		if( ((euc & 0xff00) >= 0xa100) &&
		    ((euc & 0x00ff) >= 0x00a1) &&
		    ((euc & 0xff00) <= 0xfe00) &&
		    ((euc & 0x00ff) <= 0x00fe) )
		{
			jis = (euc & 0x7f7f);
			printf("0x%2x,0x%2x\n", euc, _jis2sjis( jis ));
		}
	}
}

void	sjis2euc_12(void)
{
	int 		sjis;
	int			jis;
	int 		euc;

	printf("# SJIS,EUC\n");
	for(sjis = 0; sjis < MAX_JIS; sjis++)
	{
		jis = _sjis2jis( sjis );
		if( ((jis & 0xff00) >= 0x2100) &&
		    ((jis & 0x00ff) >= 0x0021) &&
		    ((jis & 0xff00) <= 0x7e00) &&
		    ((jis & 0x00ff) <= 0x007e) )
		{
			euc	=	(jis | 0x8080);
			printf("0x%2x,0x%2x\n", jis, euc);
		}
	}
}

//
//
//
int		main(int argc, char *argv[])
{
	int		i,j,k,l,m,n;
	int		code;
	
	if( argc < 2) { printf("usage : codemake jis2euc...\n"); exit(-1); }
	if( !strcmp(argv[1], "JIS2EUC") )  { jis2euc_12();  }
	if( !strcmp(argv[1], "EUC2JIS") )  { euc2jis_12();  }
	if( !strcmp(argv[1], "JIS2SJIS") ) { jis2sjis_12(); }
	if( !strcmp(argv[1], "SJIS2JIS") ) { sjis2jis_12(); }
	if( !strcmp(argv[1], "EUC2SJIS") ) { euc2sjis_12(); }
	if( !strcmp(argv[1], "SJIS2EUC") ) { sjis2euc_12(); }
	

}

