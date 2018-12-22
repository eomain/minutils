/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#ifndef ASH_BUILTIN
#define ASH_BUILTIN

enum ash_builtin {
    EXIT,
    ECHO,
    SLEEP,
    CD,
    HELP,
    BUILTIN,
    EXPORT
};

extern void ash_builtin_exec(int, int, const char * const *);
extern int ash_find_builtin(const char *);

#endif
