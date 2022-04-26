#include <stdio.h>
#include <stdlib.h>

void do_cat(const char *path);


int main(int argc, char *argv[]) {
	int i;

	if (argc < 2) {
		fprintf(stderr, "%s: file name not given", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; ++i) {
		do_cat(argv[i]);
	}
	exit(0);
}


void do_cat(const char *path) {
	FILE *f;
	int c;

	f = fopen(path, "r");
	if (!f) {
		perror(path);
		exit(1);
	}
	while ((c = fgetc(f)) != EOF) {
		switch (c) {
			case '\t':
				if (fputs("\\t", stdout) == EOF) exit(1);
				break;
			case '\n':
				if (fputs("$\n", stdout) == EOF) exit(1);
				break;
			default:
				if (putchar(c) == EOF) exit(1);
				break;
		}
	}
	if (fclose(f) == EOF) {
		perror(path);
		exit(1);
	}
}
