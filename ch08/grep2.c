#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *f, int match);

#define ERRMSG_BUF_SIZE 1024
#define READ_BUF_SIZE 4096

int main(int argc, char *argv[]) {
	regex_t pat;
	int opt, err;
	int regcomp_flags = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
	int match = 1;

	if (argc == 1) {
		fputs("no pattern\n", stderr);
		exit(1);
	}

	while ((opt = getopt(argc, argv, "iv")) != -1) {
		switch (opt) {
		case 'i':
			regcomp_flags |= REG_ICASE;
			break;
		case 'v':
			match = 0;
			break;
		case '?':
			printf("Usage: %s [-i] [-v] [regex] [FILE ...]\n", argv[0]);
			exit(1);
		}
	}
	
	err = regcomp(&pat, argv[optind], regcomp_flags);
	if (err != 0) {
		char buf[ERRMSG_BUF_SIZE];

		regerror(err, &pat, buf, sizeof buf);
		puts(buf);
		exit(1);
	}
	
	if (argc == optind + 1) do_grep(&pat, stdin, match);
	else {
		int i;

		for (i = optind + 1; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_grep(&pat, f, match);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(0);
}

static void do_grep(regex_t *pat, FILE *f, int match) {
	char buf[READ_BUF_SIZE];
	
	while (fgets(buf, sizeof buf, f)) {
		int match_res = regexec(pat, buf, 0, NULL, 0);
		if (match) match_res = !match_res;

		if (match_res) fputs(buf, stdout);
	}
}
