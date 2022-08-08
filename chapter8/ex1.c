#include <error.h>
#include <fcntl.h>
#include <unistd.h>

char buf[4096];

int main(int argc, char **argv) {
	register int fd;
	register int n;

	if(--argc > 0) {
		if((fd = open(*++argv, O_RDONLY, 0)) < 0)
			error(1, 0, "%s: no such file", *argv);
	} else
		fd ^= fd;

	while((n = read(fd, buf, sizeof(buf))) > 0)
		write(1, buf, n);

	while(--argc > 0) {
		close(fd);

		if((fd = open(*++argv, O_RDONLY, 0)) < 0)
			error(1, 0, "%s: no such file", *argv);
		while((n = read(fd, buf, sizeof(buf))) > 0)
			write(1, buf, n);
	}

	return 0;
}
