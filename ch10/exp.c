#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void create_file(char *filename);
static void delete_file(char *filename);
static void rename_interrupt(int rename_src);
static void unlink_interrupt(void);

int main(int argc, char *argv[]) {
	rename_interrupt(0);
	unlink_interrupt();

	exit(0);
}

static void create_file(char *filename) {
	FILE *f;

	f = fopen(filename, "w");
	if (!f) {
		perror(filename);
		exit(1);
	}
	if (fputs("from exp.h\n", f) == EOF) {
		fprintf(stderr, "error while create_file:fputs");
		exit(1);
	}
	fclose(f);
}

static void delete_file(char *filename) {
	if (unlink(filename) != 0) {
		fprintf(stderr, "error while delete_file:unlink");
		exit(1);
	}
}

static void rename_interrupt(int rename_src) {
	char *filename_src = "src";
        char *filename_dest = "dest";
	char *filename_other = "other";
	
        create_file(filename_src);
	if (!rename_src) create_file(filename_other);

        // open -> rename -> close
	// if (rename_src)
	// then rename src file
	// else rename other file
        FILE *f;
        int res_rename, c;

        f = fopen(filename_src, "r");
        if (!f) {
                perror(filename_src);
                exit(1);
        }

	if (rename_src) res_rename = rename(filename_src, filename_dest);
	else res_rename = rename(filename_other, filename_dest);

        if (res_rename < 0) {
		if (rename_src) perror(filename_src);
		else perror(filename_other);
                exit(1);
        }

        while ((c = fgetc(f)) != EOF) {
                if (putchar(c) == EOF) exit(1);
        }

        fclose(f);
	
        delete_file(filename_dest);
	if (!rename_src) delete_file(filename_src);
}

static void unlink_interrupt(void) {
	char *filename = "src";
        create_file(filename);

        // open -> rename -> close
        FILE *f;
        int c;

        f = fopen(filename, "r");
        if (!f) {
                perror(filename);
                exit(1);
        }

        if (unlink(filename) < 0) {
                perror(filename);
                exit(1);
        }

        while ((c = fgetc(f)) != EOF) {
                if (putchar(c) == EOF) exit(1);
        }

        fclose(f);
}
