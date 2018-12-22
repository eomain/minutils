/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <pwd.h>
    #include <unistd.h>
#endif

#include "env.h"
#include "io.h"
#include "var.h"

#define ENV_HOME "HOME"
#define ENV_PATH "PATH"
#define ENV_ASH_HISTORY ".ash_history"
#define DEFAULT_UNAME "[?]"
#define DEFAULT_HOST "[unknown]"
#define DEFAULT_PATH_SIZE 225
#define MAX_HOST_SIZE 64

extern int gethostname(char *, size_t);

static char *pwd = NULL;
static size_t pwd_size = DEFAULT_PATH_SIZE;
static char *dir = NULL;
static const char *home = NULL;
static const char *uname = DEFAULT_UNAME;
static const char *host = DEFAULT_HOST;
static const char *path = NULL;

void ash_prompt(void)
{
    ash_print("%s::%s %s|%c " , uname, host, dir, PROMPT);
}


const char *ash_env_get_pwd(void)
{
    return pwd;
}

size_t ash_env_get_pwd_max(void)
{
    return pwd_size;
}

const char *ash_env_get_dir(void)
{
    return dir;
}

const char *ash_env_get_home(void)
{
    return home;
}

const char *ash_env_get_uname(void)
{
    return uname;
}

const char *ash_env_get_host(void)
{
    return host;
}

const char *ash_env_get_path(void)
{
    return path;
}

void ash_env_pwd(void)
{
    pwd = getcwd(pwd, pwd_size);
    ash_var_set_builtin(ASH_PWD, pwd);
    ash_env_dir();
}

void ash_env_dir(void){
    if(home && !strcmp(pwd, home))
        dir = "~";
    else {
        if (pwd){
            size_t len = strlen(pwd);
            while (len > 0)
                if (pwd[--len] == '/'){
                    dir = &pwd[++len];
                    return;
                }
        } else
            dir = ".";
    }
}

static void ash_uname_host(void)
{
    uname = getpwuid(getuid())->pw_name;
    if (host)
        gethostname((char *)host, MAX_HOST_SIZE);
}

void ash_env_init(void)
{
    pwd_size = pathconf(".", _PC_PATH_MAX);
    if ((pwd = malloc(sizeof (char) * pwd_size)) != NULL)
        ash_env_pwd();

    host = malloc(sizeof (char) * MAX_HOST_SIZE);
    ash_uname_host();
    path = getenv(ENV_PATH);
    home = getenv(ENV_HOME);
    if (!home)
        home = getpwuid(getuid())->pw_dir;

    ash_var_set_builtin(ASH_HOST, host);
    ash_var_set_builtin(ASH_PATH, path);
    ash_var_set_builtin(ASH_PWD, pwd);
    ash_var_set_builtin(ASH_LOGNAME, uname);
    ash_var_set_builtin(ASH_HOME, home);
    ash_env_dir();
}
