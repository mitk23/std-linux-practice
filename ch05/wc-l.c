#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long count_line(const char *path);
void die(const char *s);

int main(int argc, char *argv[]) {
	int i;
	unsigned long n_line;

	if (argc < 2) {
		fprintf(stderr, "%s: file name not given\n", argv[0]);
		exit(1);
	}
	for (i = 1; i < argc; ++i) {
		n_line = count_line(argv[i]);
		printf("number of lines: %lu\n", n_line); 
	}
	exit(0);
}

#define BUFSIZE 2048

unsigned long count_line(const char *path) {
	int fd;
	unsigned char buf[BUFSIZE];
	int n;
	int i;
	int n_line = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0) die(path);

	for (;;) {
		n = read(fd, buf, sizeof buf);
		if (n < 0) die(path);
		if (n == 0) break;
		
		for (i = 0; i < sizeof buf; ++i) {
			if (buf[i] == '\n') ++n_line;
		}
	}
	if (close(fd) < 0) die(path);

	return n_line;
}	


void die(const char *s) {
	perror(s);
	exit(1);
}
