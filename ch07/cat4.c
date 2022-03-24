#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_cat(FILE *f, int escaped);

int main(int argc, char *argv[]) {
	int opt;
	int escaped = 0;

	while ((opt = getopt(argc, argv, "e")) != -1) {
		switch (opt) {
		case 'e':
			escaped = 1;
			break;
		case '?':
			fprintf(stderr, "Usage: %s [-e] [FILE ...]\n", argv[0]);
			exit(1);
		}
	}

	if (optind == argc) do_cat(stdin, escaped);
	else {
		int i;

		for (i = optind; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_cat(f, escaped);
			fclose(f);
		}
	}
	exit(0);
}

static void do_cat(FILE *f, int escaped) {
	int c;
	
	if (escaped) {
		while ((c = getc(f)) != EOF) {
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
	} else {
		while ((c = getc(f)) != EOF) {
			if (putchar(c) == EOF) exit(1);
		}
	}
}
