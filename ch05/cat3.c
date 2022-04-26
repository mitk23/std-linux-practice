#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void do_cat(int fd);
void die(const char *s);

int main(int argc, char *argv[]) {
	int i;

	if (argc == 1) {
		do_cat(STDIN_FILENO);
	}
	else {
		int fd;

		for (i = 1; i < argc; ++i) {
			fd = open(argv[i], O_RDONLY);
			if (fd < 0) die(argv[i]);
			do_cat(fd);
			close(fd);
		}
	}
	exit(0);
}

#define BUFSIZE 2048

void do_cat(int fd) {
	unsigned char buf[BUFSIZE];
	int n;

	for (;;) {
		n = read(fd, buf, sizeof buf);
		if (n < 0) die("read error");
		if (n == 0) break;
		if (write(STDOUT_FILENO, buf, n) < 0) die("write error");
	}
}	


void die(const char *s) {
	perror(s);
	exit(1);
}
