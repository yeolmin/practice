#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main()
{
	DIR *dp;
	struct dirent *p;
	dp = opendir(".");
	while( p = readdir(dp)) 
		printf("%lu %s\n", p->d_ino, p->d_name );
	closedir(dp);
	return 0;
}
