#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void tail(FILE *f, int nlines);
static char *readline(FILE *f);

int main(int argc, char *argv[]) {
	int opt;
	int nlines = 10;

	while ((opt = getopt(argc, argv, "n:")) != -1) {
		switch (opt) {
			case 'n':
				nlines = atoi(optarg);
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-n LINES] <file>", argv[0]);
				exit(1);
		}
	}

	if (optind == argc) tail(stdin, nlines);
	else {
		FILE *f;
		
		f = fopen(argv[optind], "r");
		if (!f) {
			perror(argv[optind]);
			exit(1);
		}

		tail(f, nlines);

		if (fclose(f) == EOF) {
			perror(argv[optind]);
			exit(1);
		}
	}
	exit(0);
}	


#define MAX_LINE_SIZE 1024

static void tail(FILE *f, int nlines) {
	char **ring_buffer;
	char **cur;
	char *line_buf;

#define INC(ind) do { \
	ind++; \
	if (ind >= ring_buffer + nlines) ind = ring_buffer; \
} while (0)
	
	ring_buffer = calloc(nlines, sizeof(char*));	
	if (ring_buffer == NULL) exit(1);
	cur = ring_buffer;

	while (line_buf = readline(f)) {
		*cur = line_buf;
		INC(cur);
	}
	
	// nlinesよりもfの行数が少ない場合
	if (*cur == NULL) cur = ring_buffer;

	int n;
	for (n = nlines; n > 0 && *cur; n--) {
		if (fputs(*cur, stdout) == EOF) {
			perror("fputs(3)");
			exit(1);
		}
		INC(cur);	
	}
	free(ring_buffer);
}

static char *readline(FILE *f) {
	char *line_buf;
	char *p;
	size_t len = MAX_LINE_SIZE;
	int c;

	line_buf = malloc(len * sizeof(char));
	if (line_buf == NULL) exit(1);
	p = line_buf;	

	for (;;) {
		c = fgetc(f);
		if (c == EOF) {
			if (line_buf == p) {
				free(line_buf);
				return NULL;
			}
			else break;
		}
		*p = c;
		p++;
		
		if (c == '\n') break;
	}
	*p = '\0';
	return line_buf;
}
