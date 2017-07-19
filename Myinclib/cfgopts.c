#include <stdlib.h>
#include "cfgopts.h"

char	recfm;
short	lrecl;
char	delimeter;
Boolean_T	cr;
short	adjust;

struct Config_Tag	CFG_infile[] = {
   { "recfm",		Char_Tag,	&recfm	},  /* Valid options        */
   { "lrecl",		Word_Tag,	&lrecl	},
   { NULL,			Error_Tag,	NULL	}   /* Terminating record   */
} ;

struct Config_Tag	CFG_outfile[] = {
	{ "delimeter",	Char_Tag,	&delimeter	},
	{ "cr",			Boolean_Tag,&cr			},
	{ NULL,			Error_Tag,	NULL		}
} ;

struct Config_Tag	CFG_sysin[] = {
	{ "adjust",		Word_Tag,	&adjust		},
	{ NULL,			Error_Tag,	NULL		}
} ;

int main(int argc, char *argv[])
{
   int i;


   input_config("cfgtest.ini", CFG_infile, "[infile]");
	printf("recfm=/%c/\n", recfm);
	printf("lrecl=/%d/\n", lrecl);

   input_config("cfgtest.ini", CFG_outfile, "[outfile]");
	printf("delimeter=/%c/\n", delimeter);
	printf("cr=/%s/\n", TFprint(cr));

	input_config("cfgtest.ini", CFG_sysin, "[sysin]");
	printf("adjust=/%d/\n", adjust);
   return 0;
}

