/*
 ***********************************************************************
 *	$Id: codecnv2.c,v 1.2 2006/06/25 13:25:18 hiroshi Exp $
 ***********************************************************************
 *	$Log: codecnv2.c,v $
 *	Revision 1.2  2006/06/25 13:25:18  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 */
#define		MAIN
#include	"codecnv2.h"
#include	"filecntl.h"
#include	"message.h"
#include	"cfgopts.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<process.h>


#define		MAXL_BUF		1024
#define		EXE_UNDEF		0		/* execute=NOTDEFINED */
#define		EXE_CODECONV	1		/* execute=codeconv	*/
#define		EXE_UNPACK		2		/* execute=unpack	*/
#define		EXE_FMTDMP		3		/* execute=fmtdmp	*/

#define		TYPE_UNDEF		0
#define		TYPE_CH			1
#define		TYPE_PD			2
#define		TYPE_ZD			3
#define		TYPE_NC			4
#define		TYPE_B8			5
#define		TYPE_HX			6
#define		TYPE_CP			7

    static struct FileControl   infile;
    static struct FileControl   outfile;
    static struct FileControl   jclfile = {
		"test.jcl",
		NULL,
		0,0,0,0,0,0,
		0,'#',"","","","" 
	} ;
    
    static struct FileControl   mapfile;
    static struct FileControl   logfile;
    
    static	int					giAdjust;
    static	char				gsExecute[16];
    static	int					giExecute;
    static struct CodeConvCTL	codeconvctl;

struct Config_Tag	CFG_btdtcnv[] = {
   { "execute", 	String_Tag, &gsExecute	},
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_infile[] = {
   { "recfm",		String_Tag,	&infile.s_recfm		},
   { "lrecl",		Word_Tag,	&infile.lrecl		},
   { "code_han", 	String_Tag, &infile.s_code_han	},
   { "code_zen", 	String_Tag, &infile.s_code_zen	},
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_outfile[] = {
   { "recfm",		String_Tag,	&outfile.s_recfm	},
   { "lrecl",		Word_Tag,	&outfile.lrecl		},
   { "delimiter",	Char_Tag,	&outfile.delimiter	},
   { "cr", 			Word_Tag,	&outfile.cr 		},
   { "code_han", 	String_Tag, &outfile.s_code_han },
   { "code_zen", 	String_Tag, &outfile.s_code_zen },
   { NULL,			Error_Tag,	NULL	}
} ;

struct Config_Tag	CFG_mapfile[] = {
	{ "adjust",		DWord_Tag,	&giAdjust			},
	{ NULL,			Error_Tag,	NULL				}
} ;

struct Config_Tag	CFG_codetbl[] = {
	{ "tbl_han",	String_Tag,	&codeconvctl.tbl_han_path	},
	{ "tbl_zen",	String_Tag,	&codeconvctl.tbl_zen_path	},
	{ "abend_han",	Word_Tag,	&codeconvctl.abend_han		},
	{ "abend_zen",	Word_Tag,	&codeconvctl.abend_zen		},
	{ "replace_han",	HexWord_Tag, &codeconvctl.replace_han	},
	{ "replace_zen",	DHexWord_Tag, &codeconvctl.replace_zen	},
	{ NULL,			Error_Tag,	NULL	}
} ;

char tmp[]="test";

unsigned char				buf1[MAXL_BUF];
unsigned char				buf2[MAXL_BUF];

main(int argc, char *argv[])
{

	_jobname = tmp;

	int	i,j,k,l;

	/*
	 *	ＪＣＬファイルから必要情報を取得し、チェックする
	 */
	input_config(jclfile.fname, CFG_btdtcnv,  "[btdtcnv]");
	input_config(jclfile.fname, CFG_infile,   "[infile]");
	input_config(jclfile.fname, CFG_outfile,  "[outfile]");
	input_config(jclfile.fname, CFG_mapfile,  "[mapfile]");
	input_config(jclfile.fname, CFG_codetbl,  "[codetbl]");

	/*
	 *	btdtcnv
	 */
	     if(!strcmp(gsExecute, "codeconv")) 	{	giExecute	=	EXE_CODECONV;	}
	else if(!strcmp(gsExecute, "unpack")) 	{	giExecute	=	EXE_UNPACK;		}
	else if(!strcmp(gsExecute, "fmtdmp")) 	{	giExecute	=	EXE_FMTDMP;		}
	else 									{	giExecute	=	EXE_UNDEF;		}
	



	if(     strcmp(outfile.s_code_han, "EBC") == 0) {	got_CodeHan	= CODE_EBC;	}
	else if(strcmp(outfile.s_code_han, "ASC") == 0) {	got_CodeHan	= CODE_ASC;	}
	else { ; }
	
	if(strcmp(outfile.s_code_zen,       "JIS83" ) == 0) {	got_CodeZen	= CODE_JIS83;	}
	else if(strcmp(outfile.s_code_zen,  "SJIS"  ) == 0) {	got_CodeZen	= CODE_SJIS;	}
	else if(strcmp(outfile.s_code_zen,  "KEIS83") == 0) {	got_CodeZen	= CODE_KEIS83;	}
	else if(strcmp(outfile.s_code_zen,  "JEF83" ) == 0) {	got_CodeZen	= CODE_JEF83;	}
	else { ; }
	

/*	fcntl_dmp(&infile, "[infile]", );
 *	fcntl_dmp(&outfile, "[outfile]");
 */	codeconvctl_dmp(codeconvctl, "[codetbl]");

    /*
     *  コード変換ＴＢＬの初期化
     */
	code_const_init();
	zentbl_init(codeconvctl);
	hantbl_init(codeconvctl);


	for(i = 0; i < 256; i++)
	{
		buf1[i] = i;
		printf("%d > %3d(%2x):%3d(%2x)\n", i, buf1[i], buf1[i], buf2[i], buf2[i]);
	}

	code_dmp2("HANTBL");
	
	bthancnv(buf1, 256, buf2, 256, got_CodeHan);
	
	for(i = 0; i < 256; i++)
	{
		printf("%d > %3d(%2x):%3d(%2x)\n", i, buf1[i], buf1[i], buf2[i], buf2[i]);
	}

	exit(0);


}

