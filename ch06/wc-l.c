#include <stdio.h>
#include <stdlib.h>

unsigned long count_line(const char *path);


int main(int argc, char *argv[]) {
	int i;
	unsigned long n_line;

	if (argc < 2) {
		fprintf(stderr, "%s: file name not given", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; ++i) {
		n_line = count_line(argv[i]);
		printf("number of lines: %lu\n", n_line);
	}
	exit(0);
}


unsigned long count_line(const char *path) {
	FILE *f;
	int c;
	unsigned long count = 0;
	int prev = '\n';

	f = fopen(path, "r");
	if (!f) {
		perror(path);
		exit(1);
	}
	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') ++count;
		prev = c;
	}

	if (prev != '\n') ++count;

	if (fclose(f) == EOF) {
		perror(path);
		exit(1);
	}

	return count;
}
