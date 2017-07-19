/* ---------------------------------------------------------
Name	: utility.c
Author	: Hiroshi Harasaki
Action	: usefull functions
Modify	:
 Ver 1.0	1989/01/15	マクロを関数本体の前に置く
--------------------------------------------------------- */
#include	<stdio.h>
#include	<string.h>
#include	<dos.h>
#include	<math.h>
#include	<conio.h>
#include	<time.h>
#include	"hiroshi.h"

/* ---------------------------------------------------------
	prototype declarations
--------------------------------------------------------- */
											/* checked date */
void		now( int *,int *);				/* 1989/01/15 */
void		last( int *,int *,char *);		/* 1989/01/15 */
char 		key( void);						/* 1989/01/15 */
void		scan( char *);					/* 1989/01/15 */
void		locate( int,int);				/* 1989/01/15 */
int			index( char *,char *);			/* 1989/01/15 */
int			getl( FILE *,char *);			/* 1989/01/15 */
void		get_date( struct DATE *);		/* 1989/01/15 */
int			getdir( char *);				/* 1989/01/15 */
int			cut_r_sp( char *);				/* 1989/01/15 */
int			cut_l_sp( char *);				/* 1989/01/15 */
void		ShiftLeft( char *,int);			/* 1989/01/15 */
double		rnd(void);						/* 1989/01/15 */
void		swapd(double *, double *);		/* 1989/02/07 */
void		swapi(int *, int *);			/* 1989/02/07 */
int			getopt(int ,char *[], char *);	/* 1989/04/16 */
struct tm 	get_time(void);					/* 1989/05/20 */
FILE		*f_open(char *fn, char *act);	/* 1989/11/04 */

/* ---------------------------------------------------------
Name	: key()
Action	: direct key scan
	 return character code with no echo back
*warn*	: include	<dos.h>
--------------------------------------------------------- */
char 	key()
{
union REGS	ireg,oreg;
	ireg.x.ax  = 0x0800;
	intdos(  &ireg,&oreg);
	return( oreg.h.al);
}

/* ---------------------------------------------------------
Name	: scan( word)
Action	: realtime key scan
	 set word inputed character
*warn*	: include	<stdio.h>
--------------------------------------------------------- */
void	scan( word)
char		*word;
{
char		c;
	while( 1 ){
		while( !kbhit() )		/* kbhit ｶﾞ ｱﾙﾏﾃﾞﾏﾂ */
			;
		if( (c=getch()) ==  EOL ){
			*word = '\0';
			return;
		}
		*word++ = c;
	}
}

/* ---------------------------------------------------------
Name	: locate( Low,Col)
Action	: locate cursol COLumn LOW
	1=<col=<80 , 1=<low=<20,25
--------------------------------------------------------- */
void	locate(low,col)
int		low,col;
{
	printf("\033[%d;%dH",low,col);
	return;
}

/* ---------------------------------------------------------
	Macro definiton for now() & last()
--------------------------------------------------------- */
#define		year(a)	((( unsigned)(a) >> 9) +1980)
#define		mon(a)	_montab[(((a) >> 5) & 0x0f) -1]
#define		dat(a)	((a) & 0x1f)
#define		hour(a)	(( unsigned)(a) >> 11)
#define		min(a)	(((a) >> 5) & 0x3f)
#define		sec(a)	(((a) & 0x1f) <<1)
extern char	*_montab[] = {
	"Jan","Feb","Mar","Apr","May","Jun",
	"Jul","Aug","Sep","Oct","Nov","Dec"};
/* ---------------------------------------------------------
Name	: now()
Action	: get current date & time
*warn*	: include	<dos.h>
--------------------------------------------------------- */
void	now( date,time)
int	*date,*time;
{
union REGS	ireg,oreg;

	ireg.h.ah = 0x2a;		/* 現在の日付の読みだし */
	intdos( &ireg,&oreg);
	*date = oreg.h.dl | ( oreg.h.dh << 5) | (( oreg.x.cx - 1980) << 9);

	ireg.h.ah = 0x2c;		/* 現在の時刻の読みだし */
	intdos( &ireg,&oreg);
	*time = ( oreg.h.dh >> 1) | ( oreg.h.cl << 5) | ( oreg.h.ch <<11);
}

/* ---------------------------------------------------------
Name	: last
Action	: get last accessed time of the specified file
*warn*	: include	<dos.h>
--------------------------------------------------------- */
void	last( date,time,filename)
int	*date,*time;
char	*filename;
{
int	fh;
union REGS	ireg,oreg;

	ireg.x.ax = 0x3d00;		/* ファイルのオープン */
	ireg.x.dx = (int)filename;
	intdos( &ireg,&oreg);
	fh = oreg.x.ax;			/* ファイルハンドル */

	ireg.x.ax = 0x5700;		/* ファイル時刻情報の取り出し */
	ireg.x.bx = fh;
	intdos( &ireg,&oreg);
	*date = oreg.x.dx;		/* ラストアクセスの日付 */
	*time = oreg.x.cx;		/* ラストアクセスの時刻 */

	ireg.h.ah = 0x3e;		/* ファイルのクローズ */
	ireg.x.bx = fh;			/* ファイルハンドル */
	intdos( &ireg,&oreg);
}

/* ---------------------------------------------------------
Name	: index( s1,s2)
Action	: string search program
	 this function is same as fortran's index function
*warn*	: include	<string.h>
--------------------------------------------------------- */
int	index( s1,s2)
char	*s1,*s2;
{
int	len,t;
register int	i;

	len = strlen( s2);
	t = strlen( s1) - len +1;
	for( i=0; i<t; i++)
		if( strncmp( s1+i,s2,len ) ==0 )
			return i;
	return -1;
}
/* ---------------------------------------------------------
Name	: getl
Action	: getline form file pointer
--------------------------------------------------------- */
int	getl( fp,line )
FILE	*fp;
char	*line;
{
int		i;
char		c;

	i = 0;
	while( (c = getc( fp)) != EOL && c != EOF ){
		i++;
		*line++ = c;
	}
	if( c == EOL ){
		*line++ = EOL;
		i++;
	}
	*line = '\0';
	return i;
}

/* ---------------------------------------------------------
Name	: get_date
Action	: get current date
	  現在の日付の読みだし
*warn*	: include	<dos.h>
--------------------------------------------------------- */
struct DATE {
	int	year;
	int	month;
	int	date;
	int	week;
};

void	get_date( c )
struct DATE *c;
{
union REGS ireg,oreg;
	ireg.h.ah = 0x2a;
	intdos( &ireg,&oreg);
	c->year  = oreg.x.cx;
	c->month = oreg.h.dh;
	c->date  = oreg.h.dl;
	c->week  = oreg.h.al;
	return;
}

/* ---------------------------------------------------------
Name	: getdir
Action	: wild card ﾉ ﾃﾝｶｲ
*warn*	: bdos( 0x1a,&Dta,0);	set DTA
	 DTA ｦ ｾｯﾄｼﾀｱﾄﾃﾞ getdir(....) ｦ ｺ-ﾙ
	 ＭＳ−Ｃでは／ｚｐ　ｏｐｔｉｏｎでコンパイル
	 include	<dos.h>
--------------------------------------------------------- */
#define		MAX_file	200		/* Max of file */
unsigned short	File_atr = 0x10;		/* file attribute */

struct	_dirent {
	char	f_attr;
	unsigned	f_time;
	unsigned	f_date;
	unsigned long	f_size;
	char		f_name[13];
} Entry[MAX_file];

struct	{
	char	reserv[21];
	struct	_dirent	entbuf;
} Dta;

/* -- Begin of Body -- */
int		getdir( name)
char		*name;
{
int		count = 0;
union		REGS	Ireg,Oreg;

/* Find first */
	Ireg.x.dx = (name == NULL) ? (int)"*.*" : (int)name;
	Ireg.x.cx = File_atr;
	Ireg.h.ah = (char)0x4e;		/* 一致するファイル名の検索 */
	intdos( &Ireg,&Oreg);
	if( Oreg.x.cflag == 1 ){
		fprintf(stderr,"%s: Not such file\n",name);
		return count;
	}
	memcpy( Entry + count++, &(Dta.entbuf), sizeof( struct _dirent) );
					/* Dta -> Entry　へコピー */

/* Find next */
	Ireg.h.ah = (char)0x4f;		/* 次に一致するファイル名の検索 */
	intdos( &Ireg,&Oreg);
	while( count< MAX_file && Oreg.x.cflag == 0 ){
		memcpy( Entry + count++, &(Dta.entbuf),
			sizeof( struct _dirent) );	/* Dta -> Entry copy */
		intdos( &Ireg,&Oreg);
	}
	if( Oreg.x.cflag !=0 && count > MAX_file )
		printf("Warning: file too many\n");
	return count;
}

/* ---------------------------------------------------------
Name	: cut_r_sp( s)
Action	: cut space on the right side of the string
	 and return the number of character
*warn*	: include	<string.h>
--------------------------------------------------------- */
int	cut_r_sp( s)
char	*s;
{
int		i,l;

	l = strlen( s);
        for( i = l-1; i >= 0; i--){
		if( *(s+i) != ' ' ){
			*(s+i+1) = '\0';
			return i+1;
		}
	}
	*s = '\0';
	return 0;
}

/* ---------------------------------------------------------
Name	: cut_l_sp
Action	: cut space on the left side of the string
	 and return the number of character
	 ....5....0      ....5....0
	"  345678  " -> "345678 "
	 and return 8 ( length of "345678 " )
*warn*	: include	<string.h>
--------------------------------------------------------- */
int	cut_l_sp( s)
char	*s;
{
int		i,l;

	l = strlen( s);
	for( i=0; *(s+i) == ' ' || *(s+i) == TAB && i<l; i++);
	if( i != 0 ) ShiftLeft( s,i);
	return	strlen( s);
}


/* ---------------------------------------------------------
Name	: ShiftLeft
Action	: shift string n bytes

	ShiftLeft( "123456789" ,3) is
--------------------------------------------------------- */
void	ShiftLeft( s,n)
char		*s;
int		n;
{
int	i;
	i = 0;
	while( (*(s+i) = *(s+i+n)) != TERM )
		i++;
}

/* -------------------------------------------------------------------
Name	: rnd
Action	; make a random seaquence
*warn*	: include	<math.h>
------------------------------------------------------------------- */
#define		MaxULong	0xffffffff
extern unsigned long		irnd = 1027;

double		rnd()
{
       	irnd = labs( irnd*65539);
#ifdef	DEBUG
	printf("%ld\n", irnd);
#endif
	return irnd/(double) MaxULong;
}

/* -------------------------------------------------------------------
Name	: swapd
Action	: swap a double pair
------------------------------------------------------------------- */
void		swapd(double *a, double *b)
{
double		tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* -------------------------------------------------------------------
Name	: swapi
Action	: swap a int pair
------------------------------------------------------------------- */
void		swapi(int *a, int *b)
{
int		tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* -------------------------------------------------------------------
Name	: getopt
Author	: Hiroshi Harasaki &
	Copyright (c) 1986,87,88 by Borland International Inc.
	All Rights Reserved.
Action	: Parse the command line options, System V style.
Modify	:
  Ver. 1.0	???		Turbo C original
  Ver. 1.1	1989/04/16	disable inline asm
*warn*	:
 if you compile this program by Turbo C compiler, you should define
the macro "TURBOC".
------------------------------------------------------------------- */

/*
 Standard option syntax is:

    option ::= SW [optLetter]* [argLetter space* argument]

  where
    - SW is either '/' or '-', according to the current setting
      of the MSDOS switchar (int 21h function 37h).
    - there is no space before any optLetter or argLetter.
    - opt/arg letters are alphabetic, not punctuation characters.
    - optLetters, if present, must be matched in optionS.
    - argLetters, if present, are found in optionS followed by ':'.
    - argument is any white-space delimited string.  Note that it
      can include the SW character.
    - upper and lower case letters are distinct.

  There may be multiple option clusters on a command line, each
  beginning with a SW, but all must appear before any non-option
  arguments (arguments not introduced by SW).  Opt/arg letters may
  be repeated: it is up to the caller to decide if that is an error.

  The character SW appearing alone as the last argument is an error.
  The lead-in sequence SWSW ("--" or "//") causes itself and all the
  rest of the line to be ignored (allowing non-options which begin
  with the switch char).

  The string *optionS allows valid opt/arg letters to be recognized.
  argLetters are followed with ':'.  Getopt () returns the value of
  the option character found, or EOF if no more options are in the
  command line.	 If option is an argLetter then the global optarg is
  set to point to the argument string (having skipped any white-space).

  The global optind is initially 1 and is always left as the index
  of the next argument of argv[] which getopt has not taken.  Note
  that if "--" or "//" are used then optind is stepped to the next
  argument before getopt() returns EOF.

  If an error occurs, that is an SW char precedes an unknown letter,
  then getopt() will return a '?' character and normally prints an
  error message via perror().  If the global variable opterr is set
  to false (zero) before calling getopt() then the error message is
  not printed.

  For example, if the MSDOS switch char is '/' (the MSDOS norm) and

    *optionS == "A:F:PuU:wXZ:"

  then 'P', 'u', 'w', and 'X' are option letters and 'F', 'U', 'Z'
  are followed by arguments.  A valid command line may be:

    aCommand  /uPFPi /X /A L someFile

  where:
    - 'u' and 'P' will be returned as isolated option letters.
    - 'F' will return with "Pi" as its argument string.
    - 'X' is an isolated option.
    - 'A' will return with "L" as its argument.
    - "someFile" is not an option, and terminates getOpt.  The
      caller may collect remaining arguments using argv pointers.
*/

/* ==== beginning of function ( getopt ) ==== */
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>

/* -------------------------------------------------------------------
	Gloval Variables
------------------------------------------------------------------- */
#ifndef	TURBOC		/* Hiroshi Harasaki */
int	errno;
#endif

int	optind	= 1;	/* index of which argument is next	*/
char   *optarg;		/* pointer to argument of current option */
int	opterr	= 1;	/* allow error message	*/

static	char   *letP = NULL;	/* remember next option char's location */
static	char	SW = 0;		/* DOS switch character, either '-' or '/' */
				/* switch ch is '/'			*/


int	getopt(int argc, char *argv[], char *optionS)
{
	unsigned char ch;
	char *optP;

union REGS	ireg,oreg;			/* H.H */

	if (SW == 0) {
		/* get SW using dos call 0x37 */
#ifdef	TURBOC					/* H.H */
		_AX = 0x3700;
		geninterrupt(0x21);
		SW = _DL;
#else
		ireg.x.ax = 0x3700;
		intdos( &ireg,&oreg);
		SW = oreg.h.dl;
#endif
	}

	if (argc > optind) {
		if (letP == NULL) {
			if ((letP = argv[optind]) == NULL ||
				*(letP++) != SW)  goto gopEOF;
			if (*letP == SW) {
				optind++;  goto gopEOF;
			}
		}
		if (0 == (ch = *(letP++))) {
			optind++;  goto gopEOF;
		}
		if (':' == ch  ||  (optP = strchr(optionS, ch)) == NULL)
			goto gopError;
		if (':' == *(++optP)) {
			optind++;
			if (0 == *letP) {
				if (argc <= optind)  goto  gopError;
				letP = argv[optind++];
			}
			optarg = letP;
			letP = NULL;
		} else {
			if (0 == *letP) {
				optind++;
				letP = NULL;
			}
			optarg = NULL;
		}
		return ch;
	}
gopEOF:
	optarg = letP = NULL;
	return EOF;

gopError:
	optarg = NULL;
	errno  = EINVAL;
	if (opterr)
		perror ("get command line option");
	return ('?');
} /* getopt */

/* -------------------------------------------------------------------
Name	: get_time
Action	: get current time
------------------------------------------------------------------- */
struct tm get_time(void)
{
long		t;
	time( &t);
	return *localtime( &t);
}

/* -------------------------------------------------------------------
Name	: f_open
Action	: like fopen but if file open error, immediately exit(-1)
------------------------------------------------------------------- */
FILE	*f_open(fn,act)
char		*fn,*act;
{
FILE		*fp;
    if((fp = fopen(fn,act)) == NULL) {
	fprintf(stderr,"file open error fopen(%s,%s)\n", fn,act);
	exit(-1);
    } else {
	return fp;
    }
}
