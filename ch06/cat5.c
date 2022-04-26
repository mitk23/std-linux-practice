#include <stdio.h>
#include <stdlib.h>

void do_cat(const char *path);


int main(int argc, char *argv[]) {
	int i;

	if (argc < 2) {
		fprintf(stderr, "%s: file name not given\n", argv[0]);
		exit(1);
	}
	for (i = 1; i < argc; ++i) {
		do_cat(argv[i]);
	}
	exit(0);
}

#define CHAR_SIZE sizeof(unsigned char)
#define BUF_SIZE 2048

void do_cat(const char *path) {
	FILE *f;
	unsigned char buf[BUF_SIZE];
	size_t n_read;
	size_t n_written;

	f = fopen(path, "r");
	if (!f) {
		perror(path);
		exit(1);
	}

	for (;;) {
		n_read = fread(buf, CHAR_SIZE, BUF_SIZE, f);
		if (ferror(f) != 0) {
			perror(path);
			exit(1);
		}
		n_written = fwrite(buf, CHAR_SIZE, n_read, stdout);
		if (n_written < n_read) {
			perror(path);
			exit(1);
		}
		if (n_read < BUF_SIZE) break;
	}

	if (fclose(f) == EOF) {
		perror(path);
		exit(1);
	}
}

