#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *f);

#define ERR_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	regex_t pat;
	int err;

	if (argc < 2) {
		fputs("no pattern\n", stderr);
		exit(1);
	}
	err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
	if (err != 0) {
		char buf[ERR_BUFFER_SIZE];

		regerror(err, &pat, buf, sizeof buf);
		puts(buf);
		exit(1);
	}
	if (argc == 2) do_grep(&pat, stdin);
	else {
		int i;

		for (i = 2; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_grep(&pat, f);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(0);
}

#define BUFFER_SIZE 4096

static void do_grep(regex_t *pat, FILE *f) {
	char buf[BUFFER_SIZE];

	while (fgets(buf, sizeof buf, f)) {
		if (regexec(pat, buf, 0, NULL, 0) == 0) {
			fputs(buf, stdout);
		}
	}
}
