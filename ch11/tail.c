#include <stdio.h>
#include <stdlib.h>

static void tail(FILE *f);

int main(int argc, char *argv[]) {
	if (argc != 1) {
		fprintf(stderr, "Usage: %s < infile\n", argv[0]);
		exit(1);
	}
	tail(stdin);
	exit(0);
}


#define N_RING_BUFFER 10
#define MAX_LINE_SIZE 1024

static void tail(FILE *f) {
	char ring_buffer[N_RING_BUFFER][MAX_LINE_SIZE];
	int cur = 0;
	long nlines = 0;
	int i;

	while (fgets(ring_buffer[cur], sizeof ring_buffer[cur], f)) {
		cur = (cur + 1) % N_RING_BUFFER;
		nlines++;
	}

	if (nlines <= N_RING_BUFFER) i = 0;
	else i = (cur + 1) % N_RING_BUFFER;

	for (; i != cur; i = (i + 1) % N_RING_BUFFER) {
		if (fputs(ring_buffer[i], stdout) == EOF) {
			perror("fputs(3)");
			exit(1);
		}
	}
}
