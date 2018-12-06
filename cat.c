/* Copyright 2018 - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PNAME "cat"
#define BSIZE 4096
#define BLANK ' '

static void print_err(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s\n", msg);
    exit(1);
}

static void print_errno(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s: %s\n", msg, strerror(errno));
    exit(1);
}

static int cat(const char *fname)
{
    FILE *s = fopen(fname, "r");
    if (!s)
        print_errno(fname);

    fseek(s, 0, SEEK_END);
    size_t len = ftell(s);
    fseek(s, 0, SEEK_SET);

    char *f;

    if(len < BSIZE){
        char buf[BSIZE];
        memset(buf, 0, BSIZE);
        if (fread(buf, sizeof (char), len, s) != len)
            print_errno(fname);
        f = buf;
    } else {
        f = calloc((len + 1), sizeof(char));
        if(!f)
            print_errno(fname);
        if (fread(f, sizeof (char), len, s) != len)
            print_errno(fname);
    }

    for (size_t i = 0; i < len; ++i)
        if (!isspace(f[i]) && !isprint(f[i]))
            f[i] = BLANK;
    fputs(f, stdout);
    if(f[len -1] != '\n')
        fputc('\n', stdout);
    fclose(s);
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc == 1){
        fprintf(stdout, "%s: usage: [file]\n", PNAME);
        return 0;
    } else if (argc > 2){
        print_err("additional arguments given");
    }
    return cat(argv[1]);
}
