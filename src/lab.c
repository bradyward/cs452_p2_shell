#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "lab.h"
#include <signal.h>

char *get_prompt(const char *env) // Mostly done I think
{
    char *prompt = getenv(env);

    if (prompt == NULL)
    {
        // ??? Does this nned a malloc ???
        return "Shell> ";
    }

    char *final_prompt = malloc(strlen(prompt) + 1);
    if (final_prompt == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    strcpy(final_prompt, prompt);
    return final_prompt;
}

int change_dir(char **dir) /////////////////////////
{
    // If dir is not set, or there is no directory provided, change to home
    if (dir == NULL || dir[1] == NULL)
    {
        char *home = getenv("HOME");
        if (home == NULL)
        {
            return -1;
        }
        return chdir(home);
    }
    // Change to given directory
    return chdir(dir[1]);
}

char **cmd_parse(char const *line) // 0 clue if this works lol
{
    long arg_max = sysconf(_SC_ARG_MAX);
    char **arguments = (char **)malloc(sizeof(char *) * arg_max);
    if (arguments == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    // Find total amount of arguments
    int count = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ' ')
        {
            count++;
        }
    }

    // Point arguments to a list of char pointers
    char *argument[count + 1];
    **arguments = argument;

    // Add all arguments to the list
    // Copying the string is needed so strtok doesn't modify the original
    char *copy = strdup(line);
    char *token = strtok(copy, " ");
    int index = 0;
    while (token != NULL)
    {
        argument[index] = token;
        index++;
        token = strtok(NULL, " ");
    }
    // Must be null terminated
    argument[index] = NULL;

    return arguments;
}

void cmd_free(char **line)
{
}

char *trim_white(char *line) ////////// TESTED AND WORKS !!!!!
{
    // Make a copy of the line to trim
    int lineLength = strlen(line);
    char *copy = (char *)malloc(sizeof(char) * (lineLength + 1));
    if (copy == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    copy = strcpy(copy, line);

    // Find the new front / back of the string
    int front = 0;
    int back = lineLength - 1;
    while (front < lineLength && isspace(line[front]))
    {
        front++;
    }
    while (back > front && isspace(line[back]))
    {
        back--;
    }

    // Trim the string
    copy[back + 1] = '\0';
    copy = copy + front;

    return copy;
}

bool do_builtin(struct shell *sh, char **argv)
{
}

void sh_init(struct shell *sh) // ???
{
    /* See if we are running interactively.  */
    sh->shell_terminal = NULL; // STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);

    if (sh->shell_is_interactive)
    {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp()))
            kill(-sh->shell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        /* Put ourselves in our own process group.  */
        sh->shell_pgid = getpid();
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0)
        {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);

        /* Save default terminal attributes for shell.  */
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
}

void sh_destroy(struct shell *sh) // ???
{
    // Total mallocs: 3 :
    // // A: Value returned from get_prompt
    // // B: Value returns from cmd_parse
    // // C: Value returned from trim_white

    // Total frees: 1 :
    // // A must be freed in this file. Likely in sh_destroy or the end of whatever calls it
    // // B must be freed in cmd_free
    // // C is already freed by main.c
    sh->shell_is_interactive = NULL;
    sh->shell_pgid = NULL;
    sh->shell_tmodes; // IDK what to do with this
    sh->shell_terminal = NULL;

    free(sh->prompt);
    sh->prompt = NULL;
}

void parse_args(int argc, char **argv)
{
}
