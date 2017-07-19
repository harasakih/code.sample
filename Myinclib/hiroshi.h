/* ---------------------------------------------------------
Name	: hiroshi.h
Version	: 1.0
Author	: Hiroshi Harasaki
Action	: personal header file defined by Hiroshi Harasaki
Modified: original
*warn*	User Definie should start with Upper letter
puts -> printf : not to send CR
--------------------------------------------------------- */
/* -- boolean constant -- */
#define 	True		(-1)
#define 	False		0
#define 	T		True
#define 	F		False
#define 	OR		||
#define 	AND		&&

/* -- escape code -- */
#define 	CR		'\r'		/* 0x0d */
#define 	LF		'\f'		/* 0x0a	*/
#define 	BS		'\b'		/* 0x08 */
#define 	TAB		'\t'		/* 0x09 */
#define 	ESC		\033		/* 0x1b */
#define 	EOL		'\n'		/* 0x0a */
#define 	TERM		'\0'		/* 0x00 */

/* -- escape seaquence -- */
#define 	Black()			printf("\033[30m")
#define		White()			printf("\033[37m")
#define		Blue()			printf("\033[18m")
#define		Red()			printf("\033[17m")
#define		Magenta()		printf("\033[19m")
#define		Green()			printf("\033[20m")
#define		Cyan()			printf("\033[22m")
#define		Yellow()		printf("\033[21m")

#define		Black_r()		printf("\033[40m")
#define		White_r()		printf("\033[47m")
#define		Blue_r()		printf("\033[44m")
#define		Red_r()			printf("\033[41m")
#define		Magenta_r()		printf("\033[45m")
#define		Green_r()		printf("\033[42m")
#define		Cyan_r()		printf("\033[46m")
#define		Yellow_r()		printf("\033[43m")

#define 	Normal_screen()		printf("\033[m")
#define 	Txt_cls()		printf("\033[2J")
#define 	Cursor_on()		printf("\033[>5l")
#define 	Cursor_off()		printf("\033[>5h")
#define 	Line_20()		printf("\033[>3h")
#define 	Line_25()		printf("\033[>3l")
#define		Bottom_User()		printf("\033[>1h")
#define		Bottom_System()		printf("\033[>1l")
#define		Scroll_down()		printf("\033M")
#define		Scroll_up()		printf("\033D")
#define		Nextline()		printf("\033E")
#define		Kanjimode()		printf("\033)0")
#define		Graphmode()		printf("\033)3")

#define		Locate(low,col)		printf("\033[%d;%dH", low,col)
#define		Cursor_up( n)		printf("\033[%dA", n)
#define		Cursor_down( n)		printf("\033[%dB", n)
#define		Cursor_forward( n)	printf("\033[%dC", n)
#define		Cursor_backward( n)	printf("\033[%dD", n)
#define		Say_at(low,col,str)	printf("\033[%d;%dH%s", low,col,str);

/* -- File Attribute Constant -- */
#define		Read_only	0x01
#define		Hidden		0x02
#define		System		0x04
#define		Volume		0x08
#define		Subdir		0x10
#define		Arkive		0x20

/* ---------------------------------------------------------
	physical constant & mathmatical macros
--------------------------------------------------------- */
#include	<conio.h>
#define 	Pai		3.14159265358979323846
#define 	Exp		2.71828182845904523536
#define		mod(a,b)	(a - (int)(a/b)*b)
#define		sqr(x)		((x)*(x))
#define		sgn(x)		(x >= 0 ? 1 : -1)

#define		Loop()		while(!kbhit())
