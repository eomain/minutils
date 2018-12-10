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
#define BLANK "�"

static void print_errno(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s: %s\n", msg, strerror(errno));
    exit(1);
}

static char buf[BSIZE];

static int cat(const char *fname)
{
    FILE *s = fopen(fname, "r");
    if (!s)
        print_errno(fname);

    fseek(s, 0, SEEK_END);
    size_t len = ftell(s);
    rewind(s);

    char *f;

    if(len < BSIZE){
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
    int blank = 0;
    for (size_t i = 0; i <= len; ++i)
        if (!isspace(f[i]) && !isprint(f[i]))
            ++blank;
    if (blank > 0){
        const char sp[] = BLANK;
        size_t o = sizeof (sp);
        size_t v = len + (blank * o);
        char *n = calloc( v, sizeof (char));
        if (!n)
            print_errno(fname);
        size_t k = 0;
        for (size_t i = 0; i < len; ++i){
            char c = f[i];
            if (!isspace(c) && !isprint(c)){
                memcpy(&n[k], sp, o);
                k += o -1;
            } else {
                n[k] = c;
                ++k;
            }
        }
        if (f != buf)
            free(f);
        f = n;
    }
    fputs(f, stdout);
    fclose(s);
    if (f != buf)
        free(f);
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc == 1){
        fprintf(stdout, "%s: usage: [file...]\n", PNAME);
        return 0;
    }
    for (size_t i = 1; i < argc; ++i)
        cat(argv[i]);
    return 0;
}
