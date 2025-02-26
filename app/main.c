#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../src/lab.h"

int main(int argc, char *argv[])
{
    char *c = "  a word  ";
    printf("|%s|\n", c);
    trimw(c);
    // printf("|%s|\n", cc);
}

void trimw(char *line)
{
   



    // // Two pointers initially at the beginning
    // int i = 0, j = 0;
    // // Skip leading spaces
    // while (copy[i] == ' ')
    // {
    //     i++;
    // }
    // // Shift the characters of string to remove
    // // leading spaces
    // while (copy[j++] = copy[i++]);
    // printf("|%s|\n", copy);
}

// static void explain_waitpid(int status)
// {
// if (!WIFEXITED(status))
// {
// fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(status));
// }

// if (WIFSIGNALED(status))
// {
// fprintf(stderr, "Child exited via signal %d\n", WTERMSIG(status));
// }

// if (WIFSTOPPED(status))
// {
// fprintf(stderr, "Child stopped by %d\n", WSTOPSIG(status));
// }

// if (WIFCONTINUED(status))
// {
// fprintf(stderr, "Child was resumed by delivery of SIGCONT\n");
// }
// }

// int main(int argc, char *argv[])
// {
// parse_args(argc, argv);
// struct shell sh;
// sh_init(&sh);
// char *line = (char *)NULL;
// while ((line = readline(sh.prompt)))
// {
// do nothing on blank lines don't save history or attempt to exec
// line = trim_white(line);
// if (!*line)
// {
// free(line);
// continue;
// }
// add_history(line);
// check to see if we are launching a built in command
// char **cmd = cmd_parse(line);
// if (!do_builtin(&sh, cmd))
// {
// pid_t pid = fork();
// if (pid == 0)
// {
// /*This is the child process*/
// pid_t child = getpid();
// setpgid(child, child);
// tcsetpgrp(sh.shell_terminal, child);
// signal(SIGINT, SIG_DFL);
// signal(SIGQUIT, SIG_DFL);
// signal(SIGTSTP, SIG_DFL);
// signal(SIGTTIN, SIG_DFL);
// signal(SIGTTOU, SIG_DFL);
// execvp(cmd[0], cmd);
// exit(EXIT_FAILURE);
// }
// else if (pid < 0)
// {
// If fork failed we are in trouble!
// perror("fork return < 0 Process creation failed!");
// abort();
// }

// /*
// This is in the parent put the child process into its own
// process group and give it control of the terminal
// to avoid a race condition
// */
// printf("shell:%d , child%d\n",sh.shell_pgid, pid);
// setpgid(pid, pid);
// tcsetpgrp(sh.shell_terminal, pid);
// int status;
// int rval = waitpid(pid, &status, 0);
// if (rval == -1)
// {
// fprintf(stderr, "Wait pid failed with -1\n");
// explain_waitpid(status);
// }
// cmd_free(cmd);
// get control of the shell
// tcsetpgrp(sh.shell_terminal, sh.shell_pgid);
// }
// }
// sh_destroy(&sh);
// }
