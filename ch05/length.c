#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void print_lnum(const char *path);
static int count_lnum(const char *path);
static int count_newline(const char *buf, size_t read_size);
static void die(const char *s);

int main(int argc, char *argv[]) {
	int i;

	if (argc < 2) {
		fprintf(stderr, "%s: file name not given\n", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; ++i) {
		print_lnum(argv[i]);
	}
	exit(0);
}

static void print_lnum(const char *path) {
	printf("%s: %d\n", path, count_lnum(path));
}

#define BUFFER_SIZE 2048

static int count_lnum(const char *path) {
	int fd;
	unsigned char buf[BUFFER_SIZE];
	int n, line_num = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0) die(path);
	for (;;) {
		n = read(fd, buf, sizeof buf);
		if (n < 0) die(path);
		if (n == 0) break;
		line_num += count_newline(buf, n);
	}
	if (close(fd) < 0) die(path);

	return line_num;
}

static int count_newline(const char *buf, size_t read_size) {
	int i, num_of_newline;

	for (i = 0; i < read_size; ++i) {
		if (buf[i] == '\n') ++num_of_newline;
	}

	return num_of_newline;
}

static void die(const char *s) {
	perror(s);
	exit(1);
}
