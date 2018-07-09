#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define LIST   1
#define INODE  2
#define RECUR  4
#define ALL    8

void detail_display(struct stat *buf);

int main( int argc, char **argv)
{
	int flag =0; 
	DIR *dp;
	struct dirent *p;
	struct stat buf;

	int ch;
	while( (ch = getopt(argc, argv, "liRa" ) ) != -1 )
	{
		switch(ch)
		{
			case 'l': flag |= LIST; break;
			case 'i': flag |= INODE; break;
			case 'R': flag |= RECUR; break;
			case 'a': flag |= ALL; break;
		}
	}

	dp = opendir(".");

	while ( p = readdir( dp ))
	{
		lstat( p->d_name, &buf ); 

		if( ((flag & ALL) == 0) && (p->d_name[0] == '.') )
			continue;

		if( flag & INODE )
			printf("%d\t", buf.st_ino );

		if( flag & LIST )
			detail_display(&buf);

		if( flag & LIST )
		{
			if( S_ISLNK(buf.st_mode) )  
			{
				char buff[100];
				int ret;
				ret = readlink(  p->d_name, buff, sizeof buff );
				buff[ret] = 0;
				printf(" %s -> %s", p->d_name, buff );
			}
			else
				printf(" %s", p->d_name );
		}
		else
			printf(" %s", p->d_name );

		if( flag & LIST )
			printf("\n");
	}
	printf("\n");
	closedir(dp);
	return 0;
}

void detail_display(struct stat *buf)
{
	int i;
	char perm[11] = "----------";
	char rwx[4] = "rwx";

	struct passwd *pwd;
	struct group *grp;
	struct tm *tmp;

	if( S_ISREG(buf->st_mode) ) perm[0] = '-'; 
	if( S_ISLNK(buf->st_mode) ) perm[0] = 'l'; 
	if( S_ISDIR(buf->st_mode) ) perm[0] = 'd'; 
	if( S_ISCHR(buf->st_mode) ) perm[0] = 'c'; 
	if( S_ISBLK(buf->st_mode) ) perm[0] = 'b'; 
	if( S_ISFIFO(buf->st_mode) ) perm[0] = 'p'; 
	if( S_ISSOCK(buf->st_mode) ) perm[0] = 's'; 




	for( i=0; i<9; i++ )
		if( (buf->st_mode >> (8-i)) & 1 )
			perm[i+1] = rwx[i%3];

	printf("%s", perm );
	printf(" %2d", buf->st_nlink );

	pwd = getpwuid( buf->st_uid );
	printf(" %s", pwd->pw_name );

	grp = getgrgid( buf->st_gid );
	printf(" %s", grp->gr_name );

	if( S_ISCHR(buf->st_mode) || S_ISBLK(buf->st_mode) )
	{
		printf(" %5d,%5d", (int)(buf->st_rdev>>8)&0xff, (int)buf->st_rdev&0xff);
	}
	else
		printf(" %11ld", buf->st_size );


	tmp = localtime( &buf->st_mtime );
	printf(" %04d-%02d-%02d %02d:%02d", tmp->tm_year+1900,tmp->tm_mon+1, 
			tmp->tm_mday,
			tmp->tm_hour,tmp->tm_min );

}
