/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__
    #include <unistd.h>
#endif

#include "ash.h"
#include "builtin.h"
#include "env.h"
#include "io.h"
#include "var.h"

static void ash_print_builtin(void);
static void ash_print_builtin_info(int o, const char *s);

static void ash_builtin(int argc, const char * const *argv)
{
    int status;

    if (argc == 1)
        ash_print_builtin();
    else {
        if ((status = ash_find_builtin(argv[1])) != -1)
            ash_print_builtin_info(status, argv[1]);
        else
            ash_print_err_builtin(argv[1], perr(UREG_CMD_ERR));
    }
}

static void ash_echo(int argc, const char * const *argv)
{
    if(argc > 1){
        for (size_t i = 1; i < argc -1; ++i)
            fprintf(stdout, "%s ", argv[i]);
        fputs(argv[argc -1], stdout);
        fputc('\n', stdout);
    }
}

static void ash_sleep(int argc, const char * const *argv)
{
    int status;

    if(argc == 1)
        ash_print_err_builtin(argv[0], perr(ARG_MSG_ERR));
    else {
        status = 1;
        for (size_t i = 0; i < strlen(argv[1]); ++i){
            char c = argv[1][i];
            if (!(c <= '9' && c >= '0'))
                status = 0;
        }
        if (status)
            sleep(atoi(argv[1]));
        else
            ash_print_err_builtin(argv[0], perr(TYPE_ERR));
    }
}

static void ash_cd(int argc, const char * const *argv)
{
    int status;

    if (argc > 1){
        const char *s = argv[1];
        size_t max = ash_env_get_pwd_max();
        char home_dir[max];
        if (*s == '~'){
            const char *v = s;
            if (v[1] == '/')
                ++v;
            memset(home_dir, 0, max);
            sprintf(home_dir, "%s%c%s", ash_env_get_home(), '/', ++v);
            s = home_dir;
        }
        status = chdir(s);
        if (status)
            ash_print_err_builtin(argv[0], strerror(errno));
        else
            ash_env_pwd();
    }
}

void ash_builtin_exec(int o, int argc, const char * const *argv)
{
    switch (o){
        case BUILTIN:
            ash_builtin(argc, argv);
            break;

        case EXIT:
            exit(0);
            break;

        case EXPORT:
            break;

        case ECHO:
            ash_echo(argc, argv);
            break;

        case SLEEP:
            ash_sleep(argc, argv);
            break;

        case CD:
            ash_cd(argc, argv);
            break;

        case HELP:
            ash_print_help();
            break;
    }
}

int ash_find_builtin(const char *v)
{
    switch (v[0]){
        case 'b':
            if (v[1] == 'u' &&
                v[2] == 'i' &&
                v[3] == 'l' &&
                v[4] == 't' &&
                v[5] == 'i' &&
                v[6] == 'n' &&
                !(v[7]))
                return BUILTIN;
            break;
        case 'c':
            if (v[1] == 'd' &&
                !(v[2]))
                return CD;
            break;
        case 'e':
            if (v[1] == 'x' &&
                v[2] == 'i' &&
                v[3] == 't' &&
                !(v[4]))
                return EXIT;
            else if (v[1] == 'c' &&
                     v[2] == 'h' &&
                     v[3] == 'o' &&
                     !(v[4]))
                return ECHO;
            break;
        case 'h':
            if (v[1] == 'e' &&
                v[2] == 'l' &&
                v[3] == 'p' &&
                !(v[4]))
                return HELP;
            break;
        case 's':
            if (v[1] == 'l' &&
                v[2] == 'e' &&
                v[3] == 'e' &&
                v[4] == 'p' &&
                !(v[5]))
                return SLEEP;
            break;
    }
    return -1;
}

static void ash_print_builtin_info(int o, const char *s)
{
    ash_print( PNAME ": command: ");
    switch (o){
        case BUILTIN:
            ash_print("%s :: list builtin commands\n", s);
            break;

        case CD:
            ash_print("%s [dir] :: change directory\n", s);
            break;

        case ECHO:
            ash_print("%s :: print to stdout\n", s);
            break;

        case EXIT:
            ash_print("%s :: exit shell session\n", s);
            break;

        case HELP:
            ash_print("%s :: show usage info\n", s);
            break;

        case SLEEP:
            ash_print("%s [sec] :: sleep for [sec] seconds\n", s);
            break;
    }
}

static void ash_print_builtin(void)
{
    ash_print("list of builtin commands:\n");
    ash_print("type builtin [command] for more info\n\n");
    ash_print("builtin\n");
    ash_print("cd\n");
    ash_print("echo\n");
    ash_print("exit\n");
    ash_print("help\n");
    ash_print("sleep\n");
}
