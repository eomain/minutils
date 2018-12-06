/* Copyright 2018 - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PNAME "wc"
#define BSIZE 4096
#define LIMIT 255

static void print_err(const char *msg)
{
    fprintf(stdout, PNAME ": error: %s\n", msg);
    exit(1);
}

static void print_errno(const char *msg)
{
    fprintf(stdout, PNAME ": %s: error: %s\n", msg, strerror(errno));
    exit(1);
}

static char buf[BSIZE];

static int flag = 0;

enum {
    BYTES = 1 << 0,
    LINES = 1 << 1,
    WORDS = 1 << 2
};

static void wc(const char *fname)
{
    FILE *s = fopen(fname, "r");
    if (!s)
        print_errno(fname);

    fseek(s, 0, SEEK_END);
    size_t len = ftell(s);
    fseek(s, 0, SEEK_SET);

    char *f;

    if (len < BSIZE){
        memset(buf, 0, BSIZE);
        if (fread(buf, sizeof (char), len, s) != len)
            print_errno(fname);
        f = buf;
    } else {
        f = calloc((len + 1), sizeof (char));
        if (!f)
            print_errno(fname);
        if (fread(f, sizeof (char), len, s) != len)
            print_errno(fname);
    }
    fclose(s);

    size_t lc = 1;
    size_t wcs = 0;

    if (len == 0)
        lc = wcs = 0;
    else {
        if (!isspace(f[0]))
            ++wcs;
        for (size_t i = 0; i < len; ++i){
            if (isspace(f[i]))
                ++wcs;
            if (f[i] == '\n')
                ++lc;
        }
    }
    if (!flag)
        flag = ~flag;
    if (flag & BYTES)
        fprintf(stdout, "%lu\n", len);
    if (flag & LINES)
        fprintf(stdout, "%lu\n", lc);
    if (flag & WORDS)
        fprintf(stdout, "%lu\n", wcs);

    if (f != buf)
        free(f);
}

int main(int argc, const char *argv[])
{
    if (argc == 1){
        fprintf(stdout, "%s: usage: [file]\n", PNAME);
        fprintf(stdout, "options:\n");
        fprintf(stdout, "    -b :: print byte count\n");
        fprintf(stdout, "    -l :: print line count\n");
        fprintf(stdout, "    -w :: print word count\n");
        return 0;
    }

    int count = 0;
    const char *pargs[LIMIT];
    for (size_t i = 1; i < argc; ++i)
        if (*(argv[i]) != '-'){
            if(count == LIMIT)
                print_err("exceeded max limit");
            pargs[count++] = argv[i];
        } else
            switch (argv[i][1]){
                case 'b':   flag |= BYTES;
                    break;
                case 'l':   flag |= LINES;
                    break;
                case 'w':   flag |= WORDS;
                    break;
                default:
                    fprintf(stdout, PNAME ": error: specified unrecognized argument '%s'\n", &argv[i][1]);
                    return -1;
            }
    for (size_t i = 0; i < count; ++i)
        wc(pargs[i]);
    return 0;
}
