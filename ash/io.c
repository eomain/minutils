/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ash.h"
#include "io.h"

#define MIN_BUFFER_SIZE 2096
#define MAX_BUFFER_SIZE 16384

static char buf[MIN_BUFFER_SIZE];

char *ash_scan(void)
{
    memset(buf, 0, MIN_BUFFER_SIZE);
    if(fgets(buf, MIN_BUFFER_SIZE, stdin))
        return buf;
    return NULL;
}

void ash_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void ash_print_msg(const char *msg)
{
    fprintf(stdout, PNAME " %s\n", msg);
}

void ash_print_err(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s\n", msg);
}

void ash_print_errno(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s: %s\n", msg, strerror(errno));
}

void ash_print_err_builtin(const char *pname, const char *msg)
{
    fprintf(stdout, PNAME " %s: error: %s \n", pname, msg);
}


const char *perr(int msg)
{
    switch (msg){
        case ARG_MSG_ERR:     return "expected argument";
        case TYPE_ERR:        return "incorrect value type";
        case PARSE_ERR:       return "parsed with errors";
        case UREG_CMD_ERR:    return "unrecognized command";
        case SIG_MSG_ERR:     return "abnormal termination";
        default:              return "internal error";
    }
}
