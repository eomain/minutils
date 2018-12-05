/* Copyright 2018 - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PNAME "rm"

static void print_errno(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s: %s\n", msg, strerror(errno));
    exit(1);
}

static void rm(const char* s)
{
    if(remove(s))
        print_errno(s);
}

int main(int argc, const char *argv[])
{
    if (argc == 1){
        fprintf(stdout, "%s: usage: [file ...] | [directory ...]\n", PNAME);
        return 0;
    }

    for(size_t i = 0; i < argc -1; ++i)
        rm(argv[i + 1]);
    return 0;
}
