#include <stdio.h>
#include <stdlib.h>
// getopt_long()のプロトタイプ宣言を取り込むために定数_GNU_SOURCEを定義してgetopt.hをinclude
#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

// struct optionの配列を初期化
// 配列末尾をgetopt_longに教えるために全メンバを0にした要素を最後に置く
static struct option longopts[] = {
	{"lines", required_argument, NULL, 'n'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[]) {
	int opt;
	long nlines = DEFAULT_N_LINES;

	// option解析
	while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
		switch (opt) {
		case 'n':
			nlines = atol(optarg);
			break;
		case 'h':
			fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
			exit(0);
		case '?':
			fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
			exit(1);
		}
	}
	
	// getopt, getopt_longのループから抜けた時点でoptindは非オプションの最初の引数を指している
	if (optind == argc) {
		do_head(stdin, nlines);
	} else {
		int i;

		for (i = optind; i < argc; ++i) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (!f) {
				perror(argv[i]);
				exit(1);
			}
			do_head(f, nlines);
			fclose(f);
		}
	}
	exit(0);
}

static void do_head(FILE *f, long nlines) {
	int c;

	if (nlines <= 0) return;
	while ((c = fgetc(f)) != EOF) {
		if (putchar(c) == EOF) exit(1);
		if (c == '\n') {
			nlines--;
			if (nlines == 0) return;
		}
	}
}
