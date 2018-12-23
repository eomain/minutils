/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ash.h"
#include "builtin.h"
#include "env.h"
#include "io.h"
#include "var.h"

#define DEFAULT_HISTORY_SIZE 500
#define MIN_BUFFER_SIZE 2096
#define MAX_BUFFER_SIZE 16384
#define MAX_ARGV 255

static void execute(const char *p, char *const argv[])
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        ash_print_errno(argv[0]);
    else if (pid == 0){
        if (execvp(p, argv) == -1)
            ash_print_errno(argv[0]);
        _exit(0);
    }
    else {
        wait(&status);
        if (WIFSIGNALED(status)){
            ash_print_err_builtin(argv[0], perr(SIG_MSG_ERR));
            fprintf(stderr, "%s: exit status: %d\n", argv[0], WTERMSIG(status));
        }
    }
}

static int command(int argc, const char **argv)
{
    if (argc > 1)
        for (size_t i = 1; i < argc; ++i){
            const char *s = argv[i];
            if (*(s++) == '$'){
                const char *var = ash_var_get_value( ash_find_var(s) );
                if(var)
                    argv[i] = var;
                else
                    argv[i] = "\0";
            }
        }

    const char *v = argv[0];
    if (*(v++) == '$') {
        const char *var = ash_var_get_value( ash_find_var(v) );
        if (var)
            ash_print("%s\n", var);
    } else {
        int o;
        if (( o = ash_find_builtin(argv[0])) != -1)
            ash_builtin_exec(o, argc, argv);
        else
            execute(argv[0], (char *const*)argv);
    }
    return 0;
}

static void scan(void)
{
    ash_prompt();
    char *buf;
    if ((buf = ash_scan())){
        int argc = 0;
        const char *argv[MAX_ARGV];
        memset(argv, 0, MAX_ARGV);
        int fmt = 0;
        if (!isspace(buf[0])){
            if (!(buf[0] == '\"'))
                argv[argc++] = &buf[0];
            else
                fmt = 1;
        }
        for (size_t i = 0; i < MIN_BUFFER_SIZE; ++i){
            if (buf[i] == '\n')
                buf[i] = '\0';
            if (buf[i] == '"')
                fmt = !fmt;
            if (isspace(buf[i]) && !fmt){
                buf[i] = '\0';
                if (!isspace(buf[i + 1]))
                    argv[argc++] = &buf[i + 1];
            }
        }
        if (!argc)
            return;
        command(argc, (const char **)argv);
    }
}

static void ash_main(int argc, const char **pargs)
{
    ash_env_init();

    for (;;)
        scan();
}

static int ash_option(int argc, const char **argv)
{
    for (size_t i = 0; i < argc; i++)
        if (argv[i][0] == '-' && argv[i][1] == '-'){
            const char *s = &argv[i][2];
            if (!(*s))
                ash_print_err("no option specified");
            if (!strcmp(s, "help"))
                ash_print_help();
            else if (!strcmp(s, "version"))
                ash_print_msg(VERSION);
            return 0;
        }
    return -1;
}


void ash_print_help(void)
{
    ash_print("ash: acorn shell %s\n", VERSION);
    ash_print("usage: type commands e.g. help\n\n");
    ash_print("        $$$      \n");
    ash_print("         $$      \n");
    ash_print("       $$$$$$    \n");
    ash_print("     $$$$$$$$$$  \n");
    ash_print("    $$$oooooo$$$ \n");
    ash_print("    $$oooooooo$$ \n");
    ash_print("     $oooooooo$  \n");
    ash_print("      oooooooo   \n");
    ash_print("        oooo     \n");
    ash_print("\n");
}

int main(int argc, const char *argv[])
{
    if(ash_option(--argc, ++argv))
        ash_main(argc, argv);
    return 0;
}
