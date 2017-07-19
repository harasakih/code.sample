#include	<stdio.h>
#include	<unistd.h>
#include	<time.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/file.h>
#include	<sys/types.h>


main(int argc, char *argv[])
{
	char	*resource	=	"resource.tmp";
	FILE	*fp;
	int		fd;
	time_t	t;
	char	ctime[64];
	int		i,j,k,l;
	pid_t	pid;
	char	buf[128];

	pid	= getpid();
	for(i = 0; i < 10; i++) {
		fp	=	fopen(resource, "rt");
		fd	=	fileno(fp);
		time( &t );
		strcpy(ctime, asctime(localtime(&t)));
		if( errno == 0){
			printf(     "%d -> open success[%d] errmsg[%s] :%s", pid, errno, strerror(errno), ctime); 

/* 			flock(fd, LOCK_EX); */

			time( &t );
			strcpy(ctime, asctime(localtime(&t)));
			printf(     "%d -> flock return :%s", pid, ctime); 

			break;
		} else {
			printf(     "%d -> open error[%d] errmsg[%s] Go to sleep 5 sec :%s", pid, errno, strerror(errno), ctime);
			sleep( 5 );
		}
	}
		
	time( &t );
	strcpy(ctime, asctime(localtime(&t)));

	fscanf(fp, "%s\n", buf);
	printf(     "%d -> READ[%s]\n", pid, buf);

/*	flock(fd, LOCK_UN);	*/
	fclose(fp);
	printf(     "%d -> clos errno[%d] errmsg[%s]\n", pid, errno, strerror(errno)); 
}
