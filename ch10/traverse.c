#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

struct strbuf {
	char *ptr;
	size_t len;
};

static void traverse(struct strbuf *buf);
static struct strbuf *strbuf_new(void);
static void strbuf_realloc(struct strbuf *buf, size_t size);
static void print_error(char *s);

static char *prog_name;

int main(int argc, char *argv[]) {
	struct strbuf *pathbuf;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <dir>\n", argv[0]);
		exit(1);
	}
	prog_name = argv[0];
	pathbuf = strbuf_new();
	strbuf_realloc(pathbuf, strlen(argv[1]));
	strcpy(pathbuf->ptr, argv[1]);
	
	traverse(pathbuf);

	exit(0);
}

static void traverse(struct strbuf *buf) {
	DIR *d;
	struct dirent *ent;
	struct stat st;

	d = opendir(buf->ptr);
	if (!d) {
		switch (errno) {
		case ENOTDIR:
			return;
		case ENOENT:
			return;
		case EACCES:
			puts(buf->ptr);
			print_error(buf->ptr);
			return;
		default:
			print_error(buf->ptr);
			exit(1);
		}
	}

	puts(buf->ptr);
	while ((ent = readdir(d))) {
		if (strcmp(ent->d_name, ".") == 0) continue;
		if (strcmp(ent->d_name, "..") == 0) continue;
		strbuf_realloc(buf, buf->len + 1 + strlen(ent->d_name) + 1);

		// special handling for the root
		if (strcmp(ent->d_name, "/") != 0) {
			strcat(buf->ptr, "/");
		}
		strcat(buf->ptr, ent->d_name);

		if (lstat(buf->ptr, &st) < 0) {
			switch (errno) {
			case ENOENT:
				break;
			case EACCES:
				print_error(buf->ptr);
				exit(1);
			}
		} else {
			if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
				traverse(buf);
			} else {
				puts(buf->ptr);
			}
		}
		*strrchr(buf->ptr, '/') = '\0';
	}
	closedir(d);
}

#define INIT_LEN 1024

static struct strbuf* strbuf_new(void) {
	struct strbuf *buf;

	buf = (struct strbuf*)malloc(sizeof(struct strbuf));
	if (!buf) {
		print_error("malloc(3)");
		exit(1);
	}
	buf->ptr = (char*)malloc(INIT_LEN);
	if (!buf->ptr) {
		print_error("malloc(3)");
		exit(1);
	}
	buf->len = INIT_LEN;
	return buf;
}

static void strbuf_realloc(struct strbuf *buf, size_t size) {
	char *tmp;
	
	if (buf->len > size) return;

	tmp = realloc(buf->ptr, size); 
	if (!tmp) {
		print_error("malloc(3)");
		exit(1);
	}
	buf->ptr = tmp;
	buf->len = size;
}

static void print_error(char *s) {
	fprintf(stderr, "%s: %s: %s\n", prog_name, s, strerror(errno));
}
