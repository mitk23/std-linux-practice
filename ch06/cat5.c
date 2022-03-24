#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f);

int main(int argc, char *argv[]) {
	if (argc == 1) {
	       fprintf(stderr, "%s: file name not given\n", argv[0]);
	       exit(1);
	} else {
		int i;

		for (i = 1; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_cat(f);
			if (fclose(f) != EOF) {
				perror(argv[i]);
				exit(1);
			}
		}
	}
	exit(0);
}

#define CHAR_SIZE sizeof(unsigned char)
#define BUFFER_SIZE 10 

static void do_cat(FILE *f) {
	unsigned char buf[BUFFER_SIZE];
	size_t n_read, n_written;

	for (;;) {
		n_read = fread(buf, CHAR_SIZE, sizeof buf, f);
		if (ferror(f) != 0) exit(1);
		n_written = fwrite(buf, CHAR_SIZE, n_read, stdout);
		if (n_written < n_read) exit(1);
		if (n_read < sizeof buf) break;
	}
}
