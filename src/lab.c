#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "lab.h"
#include <signal.h>

char *get_prompt(const char *env) 
{
    // Attempt to get a prompt. If one isn't given, default to "shell>"
    char *prompt = getenv(env);
    if (prompt == NULL)
    {
        prompt = "shell>";
    }

    // Copy the prompt and return it
    char *final_prompt = malloc(strlen(prompt) + 1);
    if (final_prompt == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    strcpy(final_prompt, prompt);
    return final_prompt;
}

int change_dir(char **dir)
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



char **cmd_parse(char const *line) 
{
    if (line == NULL) {
        return NULL;
    }

    // Find max argument length. Default to 4096 if sysconf fails
    long arg_max = sysconf(_SC_ARG_MAX);
    if (arg_max <= 0) {
        arg_max = 4096;
    }

    // Count the number of arguments, ensuring their combined length doesn't exceed the maximum
    int word_count = 1; // Account for initial argument. Represents numbers of arguments
    int char_count = 0; // Should represent the number of characters in the arguments. Used for malloc below
    int hit_space = 0;
    for (int i = 0; line[i] != '\0' && char_count < arg_max; i++) {
        if (line[i] == ' ') {
            hit_space = 1;
        } 
        else if (hit_space) {
            char_count++;
            hit_space = 0;
            word_count++;
        }
        else {
            char_count++;
        }
    }

    // Allocate memory for all arguments
    char **arguments = (char **)malloc(sizeof(char *) * (char_count + 1));
    if (arguments == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }

    // Add all arguments to the list
    // Copying the string is needed so strtok doesn't modify the original
    char *copy = strdup(line); // This mallocs
    if (copy == NULL)
    {
        free(arguments);
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }

    char *token = strtok(copy, " ");
    int index = 0;
    while (token != NULL)
    {
        // If we attempt to add too many args, stop and return
        if (index >= word_count)
        {
            break;
        }

        // Add the token to arguments
        arguments[index] = strdup(token);
        if (arguments[index] == NULL)
        {
            cmd_free(arguments);
            free(copy);
            fprintf(stderr, "Error: malloc failed\n");
            return NULL;
        }
        index++;
        token = strtok(NULL, " ");
    }
    // Must be null terminated
    arguments[index] = NULL;

    free(copy); // Free for copy's malloc
    return arguments;
}

void cmd_free(char **line)
{
    if (line == NULL)
    {
        return;
    }

    // Free all elements of the array first
    for (int i = 0; line[i] != NULL; i++)
    {
        free(line[i]);
        line[i] = NULL;
    }
    free(line);
    line = NULL;
}

char *trim_white(char *line)
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
    while (front < lineLength && line[front] == ' ')
    {
        front++;
    }
    while (back > front && line[back] == ' ')
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
    if (argv == NULL || argv[0] == NULL)
    {
        return 0;
    }

    if (strcmp(argv[0], "exit") == 0)
    {
        sh_destroy(sh);
        cmd_free(argv);
        exit(0);
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        if (change_dir(argv) == 0) {
            return 1;
        }
        perror("cd");
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("pwd");
        }
        return 1;
    }

    return 0;
}


void sh_init(struct shell *sh) 
{
    // This was ripped directly from the man page
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

void sh_destroy(struct shell *sh)
{
    free(sh->prompt);
    sh->prompt = NULL;
}

void parse_args(int argc, char **argv)
{
    argc = argc;
    argv = argv;
}
