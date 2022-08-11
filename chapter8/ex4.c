#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define STRLEN(x) (sizeof(x) / sizeof(*x)) - 1 /* compile-time strlen */

typedef struct _myiobuf {
	int cnt;
	char *ptr;
	char *base;
	int fd;
	int r : 1;
	int w : 1;
	int a : 1;
	int u : 1;
	int eof : 1;
	int err : 1;
} File;

#define MAXFILES 20
#define BUFSIZE 128
#define PERMS 0666
#define FEOF -1

File iob[MAXFILES] = {
	{ .fd = 0, .r = 1, .u = 0 },
	{ .fd = 1, .w = 1, .u = 0 },
	{ .fd = 2, .w = 1, .u = 1 },
};

#define fstdin (&iob[0])
#define fstdout (&iob[1])
#define fstderr (&iob[2])

File* fileopen(char *name, char *mode) {
	File *fp;
	int fd;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return 0;

	for(fp = iob; fp < iob + MAXFILES; ++fp)
		if(!(fp->r | fp->w))
			break;

	if(fp >= iob + MAXFILES)
		return 0;

	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a') {
		if((fd = open(name, O_WRONLY)) < 0)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	} else
		fd = open(name, O_RDONLY);

	if(fd < 0)
		return 0;

	fp->fd = fd;
	fp->cnt ^= fp->cnt;
	fp->base = 0;

	if(*mode == 'r')
		fp->r = 1;
	else
		fp->w = 1;

	return fp;
}

int _fillbuf(File *fp) {
	int bufsize;

	if(!fp->r)
		return FEOF;

	bufsize = (fp->u ? 1 : BUFSIZE);

	if(fp->base == 0)
		if((fp->base = malloc(bufsize)) == 0)
			return FEOF;

	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);

	if(--fp->cnt < 0) {
		if(fp->cnt == -1)
			fp->eof = 1;
		else
			fp->err = 1;

		fp->cnt ^= fp->cnt;
		return FEOF;
	}

	// (unsigned char) is used to prevent sign extension issues
	return (unsigned char)*fp->ptr;
}

int _flushbuf(File *fp) {
	int cnt;

	if(!(fp->w | fp->a))
		return FEOF;

	cnt = fp->cnt;
	fp->cnt ^= fp->cnt;
	fp->ptr = fp->base;

	if(cnt > 0 && !fp->u)
		if(write(fp->fd, fp->base, cnt) < cnt)
			return FEOF;
	return 0;
}

#define fileflush(fp) _flushbuf(fp)

int fileclose(File *fp) {
	int stat = _flushbuf(fp);
	free(fp->base);
	close(fp->fd);
	*fp = (File){0};
	return stat;
}

int fileclosenoflush(File *fp) {
	free(fp->base);
	close(fp->fd);
	*fp = (File){0};
	return 0;
}

int fileseek(File *fp, long offset, int origin) {
	register int stat;
	register int pos;

	stat ^= stat;
	pos = fp->ptr - fp->base + offset;

	if(!fp->u && fp->base) {
		if(fp->w) {
			if(_flushbuf(fp))
				return FEOF;
		} else if(fp->r) {
			if(origin != SEEK_CUR || pos < 0 || pos > fp->cnt) {
				stat = (lseek(fp->fd, offset, origin) < 0);
				fp->cnt ^= fp->cnt;
				fp->ptr = fp->base;
			} else { /* seek within buffer */
				fp->cnt -= offset;
				fp->ptr += offset;
			}
		}
	}

	if(stat == 0)
		fp->eof &= 0;

	return stat;
}

int main(void) {
	File *fp = fileopen("ex4.c", "r");
	_fillbuf(fp);
	fprintf(stdout, "#\nTEST 1\n#\n");
	write(1, fp->ptr, fp->cnt);

	fprintf(stdout, "\n#\nTEST 2\n#\n");
	fileseek(fp, 500, SEEK_SET);
	_fillbuf(fp);
	write(1, fp->ptr, fp->cnt);

	fprintf(stdout, "\n#\nTEST 3\n#\n");
	fileseek(fp, -10, SEEK_CUR);
	_fillbuf(fp);
	write(1, fp->ptr, fp->cnt);

	fprintf(stdout, "\n#\nTEST 4\n#\n");
	fileseek(fp, -200, SEEK_END);
	_fillbuf(fp);
	write(1, fp->ptr, fp->cnt);

	fileclosenoflush(fp);
	return 0;
}
