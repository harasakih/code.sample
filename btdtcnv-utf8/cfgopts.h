/*
 ***********************************************************************
 *	$Id: cfgopts.h,v 1.3 2006/07/17 07:25:30 hiroshi Exp $
 ***********************************************************************
 *	$Log: cfgopts.h,v $
 *	Revision 1.3  2006/07/17 07:25:30  hiroshi
 *	*** empty log message ***
 *	
 *	Revision 1.2  2006/06/25 13:25:17  hiroshi
 *	*** empty log message ***
 *	
 ***********************************************************************
 *
 * memicmp -> memcmp	大文字・小文字を区別
 * stricmp -> strcmp    大文字・小文字を区別
 */

#ifndef		_INC_CFGOPTS
#define		_INC_CFGOPTS


/*
**  CFGOPTS.H
*/

#ifndef Boolean_T_defined
	#define Boolean_T_defined
	#undef ERROR
	#undef FALSE
	#undef TRUE

	typedef enum {ERROR = -1,FALSE, TRUE} Boolean_T;
#endif

#ifndef TAG_TYPE_defined
	#define TAG_TYPE_defined
	typedef enum {
    	  Error_Tag,
	      Byte_Tag,
	      Boolean_Tag,
	      Word_Tag,
	      DWord_Tag,
	      OctWord_Tag,
	      DOctWord_Tag,
	      HexWord_Tag,
	      DHexWord_Tag,
	      Float_Tag,
	      Double_Tag,
	      String_Tag,
		  Char_Tag,
	      Function_Tag
	} TAG_TYPE;
#endif

#define TFprint(v) ((v) ? "TRUE" : "FALSE")

struct Config_Tag {
      char        *code;                /* Option switch        */
      TAG_TYPE    type;                 /* Type of option       */
      void        *buf;                 /* Storage location     */
};

int input_config(char *, struct Config_Tag *, char *);
int update_config(char *, struct Config_Tag *, char *);

#ifdef  MAIN

/*<<---------------[         cfgopts.c        ]------------------------/
/                                                                      /
/  Functional                                                          /
/     Description: Configuration file I/O                              /
/                                                                      /
/  Input         : Configuration file name                             /
/                  Configuration parameters in a structure             /
/                                                                      /
/  Process       : Interpret information by parameter and read or      /
/                  back to the configuration file.                     /
/                                                                      /
/  Ouput         : updated configuration file or updated structure.    /
/                                                                      /
/  Programmer    : Jeffry J. Brickley                                  /
/                                                                      /
/                                                                      /
/---------------------------------------------------------------------*/
#define       Assigned_Revision 950802
/*-------------------------[ Revision History ]------------------------/
/ Revision 1.0.0  :  Original Code by Jeffry J. Brickley               /
/          1.1.0  : added header capability, JJB, 950802
/------------------------------------------------------------------->>*/
/*  Please keep revision number current.                              */
#define       REVISION_NO "1.1.0"

/*---------------------------------------------------------------------/
/
/  Description:  CfgOpts is based on GETOPTS by Bob Stout.  It will
/                process a configuration file based one words and
/                store it in a structure pointing to physical data
/                area for each storage item.
/  i.e. ???.CFG:
/    Port=1
/    work_space=C:\temp
/    menus=TRUE
/    user=Jeffry Brickley
/  will write to the following structure:
/    struct Config_Tag configs[] = {
/    {"port",       Word_Tag,    &port_number},
/    {"work_space", String_Tag,  &work_space},
/    {"menus",      Boolean_Tag, &menu_flag},
/    {"user",       String_Tag,  &User_name},
/    {NULL,         Error_Tag,   NULL}
/    };
/  Note that the structure must always be terminated by a NULL row as
/     was the same with GETOPTS.  This however is slightly more
/     complicated than scaning the command line (but not by much) for
/     data as there can be more variety in words than letters and an
/     number of data items limited only by memory.  Currently CfgOpts
/     is not case sensitive, but this can be changed by replacing all
/     "stricmp" functions with "strcmp" functions.
/
/  Like the original code from which this was taken, this is released
/  to the Public Domain.  I cannot make any guarentees other than these
/  work for me and I find them usefull.  Feel free to pass these on to
/  a friend, but please do not charge him....
/
/---------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * FCOPY.C - copy one file to another.  Returns the (positive)
 *           number of bytes copied, or -1 if an error occurred.
 * by: Bob Jarvis
 */

#define BUFFER_SIZE 1024

/*---------------------------------------------------------------------/
/   copy one file to another.
/---------------------------------------------------------------------*/
/*>>------[       fcopy()      ]-------------[ 08-02-95 14:02PM ]------/
/ return value:
/     long                    ; Number of bytes copied or -1 on error
/ parameters:
/     char *dest              ; Destination file name
/     char *source            ; Source file name
/-------------------------------------------------------------------<<*/
long fcopy(char *dest, char *source)
{
   FILE *d, *s;
   char *buffer;
   size_t incount;
   long totcount = 0L;

   s = fopen(source, "rb");
   if(s == NULL)
      return -1L;

   d = fopen(dest, "wb");
   if(d == NULL)
   {
      fclose(s);
      return -1L;
   }

   buffer = (char *)malloc(BUFFER_SIZE);
   if(buffer == NULL)
   {
      fclose(s);
      fclose(d);
      return -1L;
   }

   incount = fread(buffer, sizeof(char), BUFFER_SIZE, s);

   while(!feof(s))
   {
      totcount += (long)incount;
      fwrite(buffer, sizeof(char), incount, d);
      incount = fread(buffer, sizeof(char), BUFFER_SIZE, s);
   }

   totcount += (long)incount;
   fwrite(buffer, sizeof(char), incount, d);

   free(buffer);
   fclose(s);
   fclose(d);

   return totcount;
}

static char _line[256];
/* 2006/07/15 comment out 
static int  *Funct(char *);
*/

/*---------------------------------------------------------------------/
/   reads from an input configuration (INI) file.
/---------------------------------------------------------------------*/
/*>>------[   input_config()   ]-------------[ 08-02-95 14:02PM ]------/
/ return value:
/     int                     ; number of records read or -1 on error
/ parameters:
/     char *filename          ; filename of INI style file
/     struct Config_Tag configs[]; Configuration structure
/     char *header            ; INI header name (i.e. "[TEST]")
/-------------------------------------------------------------------<<*/
int input_config(char *filename, struct Config_Tag configs[], char *header)
{
   struct Config_Tag *ptr;
   int count=0,_lineno=0,temp;
   FILE *file;
   char *fptr,*tok,*next;
   void *function();

   file=fopen(filename,"r");
   if ( file==NULL ) return ERROR;			/* return error designation. */
   if ( header!=NULL )
      do
   {
      fptr=fgets(_line,255,file);			/* get input line	*/
   }
   while ( memcmp(_line,header,strlen(header)) && !feof(file));		/* memicmp -> memcmp */
   if ( !feof(file) ) do {
      fptr=fgets(_line,255,file);			/* get input line	*/
      if ( fptr==NULL ) continue;
      _lineno++;
      if ( _line[0]=='#' ) continue;		/* skip comments	*/
      if ( _line[0]=='[' ) continue;		/* skip next header	*/
      tok=strtok(_line,"=\n\r");			/* get first token	*/
      if ( tok!=NULL )
      {
         next=strtok(NULL,"=\n\r");			/* get actual config information	*/
         for ( ptr=configs;ptr->buf;++ptr )	/* scan for token	*/
         {
            if ( !strcmp( tok , ptr->code ) )	/* got a match?	*/
            {
               switch ( ptr->type )			/* check type		*/
               {
               case Boolean_Tag:
                  if (!strcmp(next,"FALSE"))
                     *((Boolean_T *)(ptr->buf)) = FALSE;
                  else if (!strcmp(next,"TRUE"))
                     *((Boolean_T *)(ptr->buf)) = TRUE;
                  ++count;
                  break;

               case Byte_Tag:
                  sscanf(next, "%d", &temp);
                  *((char *)(ptr->buf))=(char)temp;
                  ++count;
                  break;

               case Word_Tag:
                  sscanf(next, "%hd", (short *)(ptr->buf));		/* 2006/07/15 int -> short */
                  ++count;
                  break;

               case DWord_Tag:
                  sscanf(next, "%ld", (long *)(ptr->buf));
                  ++count;
                  break;

               case OctWord_Tag:
                  sscanf(next, "%ho", (short *)(ptr->buf));		/* 2006/07/15 int -> short */
                  ++count;
                  break;

               case DOctWord_Tag:
                  sscanf(next, "%lo", (long *)(ptr->buf));
                  ++count;
                  break;

               case HexWord_Tag:
                  sscanf(next, "%hx", (short *)(ptr->buf));		/* 2006/07/15 int -> short */
                  ++count;
                  break;

               case DHexWord_Tag:
                  sscanf(next, "%lx", (long *)(ptr->buf));
                  ++count;
                  break;

               case Float_Tag:
                  sscanf(next, "%g", (float *)ptr->buf);
                  ++count;
                  break;

               case Double_Tag:
                  sscanf(next, "%lg", (double *)ptr->buf);
                  ++count;
                  break;

               case String_Tag:
                  strcpy((char *)ptr->buf, next);
                  ++count;
                  break;

               case Char_Tag:
                  sscanf(next, "%c", (char *)ptr->buf);
				  ++count;
				  break;

               case Function_Tag:
               case Error_Tag:
               default:
                  printf("Error in Config file %s on line %d\n",
                  filename,_lineno);
                  break;
               }
            }

         }
      }
   }
   while ( fptr!=NULL && _line[0]!='[');
   fclose(file);
   return count;
}

/*---------------------------------------------------------------------/
/   updates an input configuration (INI) file from a structure.
/---------------------------------------------------------------------*/
/*>>------[   update_config()  ]-------------[ 08-02-95 14:02PM ]------/
/ return value:
/     int                     ; Number of records read & updated
/ parameters:
/     char *filename          ; filename of INI file
/     struct Config_Tag configs[]; Configuration structure
/     char *header            ; INI header name (i.e. "[TEST]")
/-------------------------------------------------------------------<<*/
int update_config(char *filename, struct Config_Tag configs[], char *header)
{
   struct Config_Tag *ptr;
   int count=0,_lineno=0,temp;
   FILE *infile,*outfile;
   char *fptr,*tok,*next;
   static char *true_false[]={
      "ERROR","FALSE","TRUE"    };

   infile=fopen(filename,"r");
   if ( infile==NULL ) {
      outfile=fopen("temp.$$$","w");
      if ( outfile==NULL )
         return ERROR;						/* return error designation.	*/
      if ( header!=NULL )
      {
         fprintf(outfile,"%s\n",header);
      }
      for ( ptr=configs;ptr->buf;++ptr )	/* scan for token				*/
      {
         fprintf(outfile,"%s=",ptr->code);
         switch ( ptr->type )				/* check type					*/
         {
         case Boolean_Tag:
            fprintf(outfile,"%s\n",
            true_false[*((Boolean_T *)(ptr->buf))+1]);
            ++count;
            break;

         case Byte_Tag:
            temp=(int)*((char *)(ptr->buf));
            fprintf(outfile, "%d\n", temp);
            ++count;
            break;

         case Word_Tag:
            fprintf(outfile, "%hd\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DWord_Tag:
            fprintf(outfile, "%ld\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case OctWord_Tag:
            fprintf(outfile, "%ho\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DOctWord_Tag:
            fprintf(outfile, "%lo\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case HexWord_Tag:
            fprintf(outfile, "%hx\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DHexWord_Tag:
            fprintf(outfile, "%lx\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case Float_Tag:
            fprintf(outfile, "%g\n", *((float *)ptr->buf));
            ++count;
            break;

         case Double_Tag:
            fprintf(outfile, "%lg\n", *((double *)ptr->buf));
            ++count;
            break;

         case String_Tag:
            fprintf(outfile, "%s\n",(char *)ptr->buf);
            ++count;
            break;

         case Char_Tag:
            fprintf(outfile, "%c\n", *((char *)ptr->buf));
			++count;
			break;

         case Error_Tag:
         case Function_Tag:
         default:
            printf("Error in Config structure (Contact author).\n");
            break;
         }

      }

      fclose(outfile);
      fcopy(filename,"temp.$$$");
      remove("temp.$$$");
      return count;
   }
   outfile=fopen("temp.$$$","w");
   if ( outfile==NULL ) {
      fclose(infile);
      return ERROR;							/* return error designation.	*/
   }
   if ( header!=NULL )
   {
      do
      {
         fptr=fgets(_line,255,infile);		/* get input line				*/
         fprintf(outfile,"%s",_line);
      }
      while ( memcmp(_line,header,strlen(header)) && !feof(infile));		/* memicmp -> memcmp */
   }
   if ( feof(infile) ) {
      if ( header!=NULL )
      {
         fprintf(outfile,"%s\n",header);
      }
      for ( ptr=configs;ptr->buf;++ptr )	/* scan for token				*/
      {
         fprintf(outfile,"%s=",ptr->code);
         switch ( ptr->type )				/* check type					*/
         {
         case Boolean_Tag:
            fprintf(outfile,"%s\n",
            true_false[*((Boolean_T *)(ptr->buf))+1]);
            ++count;
            break;

         case Byte_Tag:
            temp=(int)*((char *)(ptr->buf));
            fprintf(outfile, "%d\n", temp);
            ++count;
            break;

         case Word_Tag:
            fprintf(outfile, "%hd\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DWord_Tag:
            fprintf(outfile, "%ld\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case OctWord_Tag:
            fprintf(outfile, "%ho\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DOctWord_Tag:
            fprintf(outfile, "%lo\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case HexWord_Tag:
            fprintf(outfile, "%hx\n", *((int *)(ptr->buf)));
            ++count;
            break;

         case DHexWord_Tag:
            fprintf(outfile, "%lx\n", *((long *)(ptr->buf)));
            ++count;
            break;

         case Float_Tag:
            fprintf(outfile, "%g\n", *((float *)ptr->buf));
            ++count;
            break;

         case Double_Tag:
            fprintf(outfile, "%lg\n", *((double *)ptr->buf));
            ++count;
            break;

         case String_Tag:
            fprintf(outfile, "%s\n",(char *)ptr->buf);
            ++count;
            break;

         case Char_Tag:
            fprintf(outfile, "%c\n", *((char *)ptr->buf));
			++count;
			break;

         case Error_Tag:
         case Function_Tag:
         default:
            printf("Error in Config structure (Contact author).\n");
            break;
         }

      }
   }
   else {
      do
      {
         fptr=fgets(_line,255,infile);			/* get input line	*/
         if ( fptr==NULL ) continue;
         _lineno++;
         if ( _line[0]=='#' ) {
            fprintf(outfile,"%s",_line);
            continue;							/* skip comments	*/
         }
         tok=strtok(_line,"=\n\r");				/* get first token	*/
         if ( tok!=NULL )
         {
            next=strtok(_line,"=\n\r");			/* get actual config information	*/
            for ( ptr=configs;ptr->buf;++ptr )	/* scan for token	*/
            {
               if ( !strcmp( tok , ptr->code ) )	/* got a match?	*/
               {
                  fprintf(outfile,"%s=",tok);
                  switch ( ptr->type )			/* check type		*/
                  {
                  case Boolean_Tag:
                     fprintf(outfile,"%s\n",
                     true_false[*((Boolean_T *)(ptr->buf))+1]);
                     ++count;
                     break;

                  case Byte_Tag:
                     temp=(int)*((char *)(ptr->buf));
                     fprintf(outfile, "%d\n", temp);
                     ++count;
                     break;

                  case Word_Tag:
                     fprintf(outfile, "%hd\n", *((int *)(ptr->buf)));
                     ++count;
                     break;

                  case DWord_Tag:
                     fprintf(outfile, "%ld\n", *((long *)(ptr->buf)));
                     ++count;
                     break;

                  case OctWord_Tag:
                     fprintf(outfile, "%ho\n", *((int *)(ptr->buf)));
                     ++count;
                     break;

                  case DOctWord_Tag:
                     fprintf(outfile, "%lo\n", *((long *)(ptr->buf)));
                     ++count;
                     break;

                  case HexWord_Tag:
                     fprintf(outfile, "%hx\n", *((int *)(ptr->buf)));
                     ++count;
                     break;

                  case DHexWord_Tag:
                     fprintf(outfile, "%lx\n", *((long *)(ptr->buf)));
                     ++count;
                     break;

                  case Float_Tag:
                     fprintf(outfile, "%g\n", *((float *)ptr->buf));
                     ++count;
                     break;

                  case Double_Tag:
                     fprintf(outfile, "%lg\n", *((double *)ptr->buf));
                     ++count;
                     break;

                  case String_Tag:
                     fprintf(outfile, "%s\n",(char *)ptr->buf);
                     ++count;
                     break;

                  case Char_Tag:
                     fprintf(outfile, "%c\n", *((char *)ptr->buf));
					 ++count;
					 break;

                  default:
                     printf("Error in Config file %s on line %d\n",
                     filename,_lineno);
                     break;
                  }
               }

            }
         }
      }
      while ( fptr!=NULL );
   }
   fclose(infile);
   fclose(outfile);
   fcopy(filename,"temp.$$$");
   remove("temp.$$$");
   return count;
}

#endif		/* ifdef	MAIN */

#endif		/* ifndef	_INC_CFGOPTS */
