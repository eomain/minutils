/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#ifndef ASH_IO
#define ASH_IO

#define PROMPT '$'

enum ash_errno {
    ARG_MSG_ERR,
    TYPE_ERR,
    PARSE_ERR,
    UREG_CMD_ERR,
    SIG_MSG_ERR
};

extern char *ash_scan(void);
extern void  ash_print(const char *, ...);
extern void  ash_print_msg(const char *);
extern void ash_print_err(const char *);
extern void ash_print_errno(const char *);
extern void ash_print_err_builtin(const char *, const char *);
extern const char *perr(int);

#endif
