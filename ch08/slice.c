#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_slice(regex_t *pat, FILE *f);

#define ERR_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	regex_t pat;
	int err;

	if (argc < 2) {
		fputs("no pattern\n", stderr);
		exit(1);
	}
	err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NEWLINE);
	if (err != 0) {
		char buf[ERR_BUFFER_SIZE];

		regerror(err, &pat, buf, sizeof buf);
		puts(buf);
		exit(1);
	}
	if (argc == 2) do_slice(&pat, stdin);
	else {
		int i;

		for (i = 2; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_slice(&pat, f);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(0);
}

#define BUFFER_SIZE 4096

static void do_slice(regex_t *pat, FILE *f) {
	char buf[BUFFER_SIZE];

	while (fgets(buf, sizeof buf, f)) {
		regmatch_t pmatch[1];

		if (regexec(pat, buf, 1, pmatch, 0) == 0) {
			char *str = buf + pmatch[0].rm_so;
			regoff_t len = pmatch[0].rm_eo - pmatch[0].rm_so;

			fwrite(str, len, 1, stdout);
			fputc('\n', stdout);	
		}
	}
}
