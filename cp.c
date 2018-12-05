/* Copyright 2018 - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PNAME "cp"
#define BSIZE 4096

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

static int cp(const char *src, const char *dest)
{
    if (!strcmp(src, dest))
        print_err("destination same as source");

    FILE *s = fopen(src, "r");
    if (!s)
        print_errno(src);

    FILE *d = fopen(dest, "w");
    if (!d)
        print_errno(dest);

    fseek(s, 0, SEEK_END);
    size_t len = ftell(s);
    fseek(s, 0, SEEK_SET);

    if(len <= BSIZE){
        char buf[BSIZE];
        if (fread(buf, sizeof (char), len, s) != len)
            print_errno(src);
        if (fwrite((const void *) buf,sizeof (char), len, d) != len)
            print_errno(dest);
    } else {
        char *mbuf = malloc(sizeof (char) * len);
        if(!mbuf)
            print_errno("no memory");
        if (fread(mbuf, sizeof (char), len, s) != len)
            print_errno(src);
        if (fwrite((const void *) mbuf, sizeof (char), len, d) != len)
            print_errno(dest);
    }
    fclose(s);
    fclose(d);
    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc == 1){
        fprintf(stdout, "%s: usage: src [file], dest [file]\n", PNAME);
        return 0;
    } else if (argc == 2){
        print_err("expected argument destination");
    }
    return cp(argv[1], argv[2]);
}
