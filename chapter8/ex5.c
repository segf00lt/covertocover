#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void dirwalk(char *dir, void (*fn)(char *)) {
	char name[PATH_MAX];
	struct dirent *ent;
	DIR *dp;

	if((dp = opendir(dir)) == 0)
		error(1, 0, "dirwalk: can't open %s", dir);

	while((ent = readdir(dp)) != 0) {
		if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;

		if(strlen(dir) + strlen(ent->d_name) + 2 > sizeof(name))
			error(1, 0, "dirwalk: name %s/%s too long", dir, ent->d_name);
		else {
			sprintf(name, "%s/%s", dir, ent->d_name);
			fn(name);
		}
	}

	closedir(dp);
}

void finfo(char *name) {
	struct stat buf;
	
	if(stat(name, &buf) == -1)
		error(1, 0, "%s: could not stat", name);

	if(buf.st_mode & S_IFDIR)
		dirwalk(name, finfo);

	printf("%s:\n\tuid: %i\n\tgid: %i\n\tacc: %li\n\tmod: %li\n\tsiz: %li\n",
			name, buf.st_uid, buf.st_gid, buf.st_atim, buf.st_mtim, buf.st_size);
}

int main(int argc, char **argv) {
	if(argc == 1)
		finfo(".");
	else
		while(--argc > 0)
			finfo(*++argv);

	return 0;
}
