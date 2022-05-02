#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

struct cmd {
	char **argv;
	long argc;
	long capa;
};


static struct cmd *read_cmd(void);
static struct cmd *parse_cmd(char *cmdline);
static void call_cmd(struct cmd *cmd);
static void free_cmd(struct cmd *cmd);


#define PROMPT "$"
#define MAX_LINE_SIZE 1024
#define MAX_N_CMD 16

int main(int argc, char *argv[]) {
	for (;;) {
		struct cmd *cmd;

		fprintf(stdout, "%s ", PROMPT);
		fflush(stdout);
		cmd = read_cmd();
		if (cmd->argc > 0) call_cmd(cmd);

		free_cmd(cmd);
	}
	exit(0);
}


static struct cmd *read_cmd(void) {
	static char line_buf[MAX_LINE_SIZE];

	if (fgets(line_buf, sizeof line_buf, stdin) == NULL) {
		exit(0);
	}

	return parse_cmd(line_buf);
}


static struct cmd *parse_cmd(char *cmdline) {
	char *p = cmdline;
	struct cmd *cmd;

	cmd = malloc(sizeof(struct cmd));
	cmd->argv = malloc(sizeof(char*) * MAX_N_CMD);
	cmd->argc = 0;
	cmd->capa = MAX_N_CMD;
	
	while (*p) {
		while (*p && isspace((int)*p)) *p++ = '\0';
		if (*p) {
			cmd->argv[cmd->argc] = p;
			cmd->argc++;
		}
		while (*p && !isspace((int)*p)) p++;
	}
	cmd->argv[cmd->argc] = NULL;
	return cmd;
}


static void call_cmd(struct cmd *cmd) {
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("fork(2)");
		exit(1);
	}
	if (pid > 0) {
		waitpid(pid, NULL, 0);
	} else {
		execvp(cmd->argv[0], cmd->argv);
		fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
		exit(1);
	}
}


static void free_cmd(struct cmd *cmd) {
	free(cmd->argv);
	free(cmd);
}
