/* ----------------------------------------------------------------------------
   ﾌｧｲﾙ	メッセージ送出関連の関数
   ---------------------------------------------------------------------------- */
#ifndef     _INC_MESSAGE
#define     _INC_MESSAGE

#include    <stdio.h>
#include    <time.h>
#include    <string.h>
/*
 #include	<conio.h>
 */

#define MAX_MSGBUF          128     /* ﾒｯｾｰｼﾞﾊﾞｯﾌｧの最大長 */

#define csl_msgs(msg)           _msg(stderr, _jobname, "STRT", msg)
#define csl_msge(msg)           _msg(stderr, _jobname, "END ", msg)
#define csl_msgr(msg)           _msg(stderr, _jobname, "ERR ", msg)
#define csl_msg(msg)            _msg(stderr, _jobname, "MSG ", msg)
#define csl_msgb(msg)           _msg(stderr, _jobname, "ABND", msg)
#define csl_msga(argv)          _msga(stderr, _jobname,"ARG ", argv)
#define csl_msgc(fil, cnt)      _msgiocnt(stderr, _jobname, "CNT ", fil, cnt)
#define csl_inq(msg)            { _msg(stderr, _jobname, "INQ", msg); getchar(); }

#define fil_msgs(fp, msg)       _msg(fp, _jobname, "STRT", msg)
#define fil_msge(fp, msg)       _msg(fp, _jobname, "END ", msg)
#define fil_msgr(fp, msg)       _msg(fp, _jobname, "ERR ", msg)
#define fil_msg(fp, msg)        _msg(fp, _jobname, "MSG ", msg)
#define fil_msga(fp, argv)      _msga(fp, _jobname,"ARG ", argv)
#define fil_msgc(fp, fil, cnt)  _msgiocnt(fp, _jobname, "CNT ", fil, cnt)

/* ----------------------------------------------------------------------------
    構造体の定義
   ---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
    関数プロトタイプ宣言
   ---------------------------------------------------------------------------- */
int     _msg (FILE *fp, char *job, char *msgid, char *msg);
int     _msga(FILE *fp, char *job, char *msgid, char **argv);
int     _msgiocnt(FILE *fp, char *job, char *msgid, char *msg, int cnt);
char    *now(void);

#ifdef  MAIN

/* ----------------------------------------------------------------------------
    外部変数(Static)の定義
   ---------------------------------------------------------------------------- */
static char _msgbuf[MAX_MSGBUF];
static char     *_jobname;

/* ----------------------------------------------------------------------------
    関数の定義
   ---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
   処理
   戻値
   引数
   注意
   ---------------------------------------------------------------------------- */
int _msg(FILE *fp, char *job, char *msgid, char *msg)
{
        fprintf(fp, "JOB[%-8s] %-4s %s %s\n", job, msgid, now(), msg);
        return 0;
}

/* ----------------------------------------------------------------------------
   処理
   戻値
   引数
   注意
   ---------------------------------------------------------------------------- */
int _msgiocnt(FILE *fp, char *job, char *msgid, char *file, int iocnt)
{
        fprintf(fp, "JOB[%-8s] %-4s %s %s=%d\n", job, msgid, now(), file, iocnt);
        return 0;
}

/* ----------------------------------------------------------------------------
   処理
   戻値
   引数
   注意
   ---------------------------------------------------------------------------- */
int _msga(FILE *fp, char *job, char *msgid, char *argv[])
{
        struct tm   *a_time;
        time_t a_clock;
        char        **cp;

        time( &a_clock );
        a_time = localtime( &a_clock );
        fprintf(fp, "JOB[%-8s] %-4s %s ", job, msgid, now());
        cp = argv;
        while( *++cp ) {
                fprintf(fp, "%s ", *cp);
        }
        fprintf(fp, "\n");
        return 0;
}

/* ----------------------------------------------------------------------------
   処理
   戻値
   引数
   注意
   ---------------------------------------------------------------------------- */
char*   now()
{
        struct tm   *a_time;
        time_t a_clock;
        static char stime[20];

        time( &a_clock );
        a_time = localtime( &a_clock );
        sprintf(stime, "%4d/%02d/%02d/%02d:%02d:%02d",
                a_time->tm_year + 1900,
                a_time->tm_mon + 1,
                a_time->tm_mday,
                a_time->tm_hour,
                a_time->tm_min,
                a_time->tm_sec );
        stime[19] = 0x0;

        return stime;
}

#else
/* ----------------------------------------------------------------------------
    外部変数の宣言
   ---------------------------------------------------------------------------- */

extern char _msgbuf[];
extern char     *_jobname;

#endif      /* ifdef	MAIN */

#endif      /* ifndef	_INC_MESSAGE */
