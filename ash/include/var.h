/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#ifndef ASH_VAR
#define ASH_VAR

struct ash_variable;

enum ash_builtin_variable {
    ASH_VERSION = 0x00,
    ASH_HOST    = 0x01,
    ASH_PATH    = 0x02,
    ASH_HOME    = 0x03,
    ASH_PWD     = 0x04,
    ASH_LOGNAME = 0x05
};

extern struct ash_variable *ash_var_find_builtin(int);
extern struct ash_variable *ash_find_var(const char *);
extern void ash_var_set_builtin(int, const char *);
extern const char *ash_var_get_value(struct ash_variable *);

#endif
