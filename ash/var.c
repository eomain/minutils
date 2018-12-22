/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#include <stddef.h>

#include "ash.h"
#include "var.h"

struct ash_variable {
    int id;
    const char *name;
    const char *value;
};

static struct ash_variable ash_vars[] = {
    {
        .id = ASH_VERSION,
        .name = "VERSION",
        .value = VERSION
    },

    {
        .id = ASH_HOST,
        .name = "HOST",
        .value = NULL
    },

    {
        .id = ASH_PATH,
        .name = "PATH",
        .value = NULL
    },

    {
        .id = ASH_HOME,
        .name = "HOME",
        .value = NULL
    },

    {
        .id = ASH_PWD,
        .name = "PWD",
        .value = NULL
    },

    {
        .id = ASH_LOGNAME,
        .name = "LOGNAME",
        .value = NULL
    }
};

struct ash_variable *ash_var_find_builtin(int o)
{
    if (o < sizeof (ash_vars) / sizeof (ash_vars[0]))
        return &ash_vars[o];
    return NULL;
}

struct ash_variable *ash_find_var(const char *s)
{
    const char *v = s;
    switch(v[0]){
        case 'V':
            if (v[1] == 'E' &&
                v[2] == 'R' &&
                v[3] == 'S' &&
                v[4] == 'I' &&
                v[5] == 'O' &&
                v[6] == 'N' &&
                !(v[7]))
                return ash_var_find_builtin(ASH_VERSION);
            break;

        case 'H':
            if (v[1] == 'O'){
                if (v[2] == 'S' &&
                    v[3] == 'T' &&
                    !(v[4]))
                    return ash_var_find_builtin(ASH_HOST);
                else if (v[2] == 'M' &&
                         v[3] == 'E' &&
                         !(v[4]))
                    return ash_var_find_builtin(ASH_HOME);
            }
            break;

        case 'P':
            if (v[1] == 'A' &&
                v[2] == 'T' &&
                v[3] == 'H' &&
                !(v[4]))
                return ash_var_find_builtin(ASH_PATH);
            else if (v[1] == 'W' &&
                     v[2] == 'D' &&
                     !(v[3]))
                return ash_var_find_builtin(ASH_PWD);
            break;

        case 'L':
            if (v[1] == 'O' &&
                v[2] == 'G' &&
                v[3] == 'N' &&
                v[4] == 'A' &&
                v[5] == 'M' &&
                v[6] == 'E' &&
                !(v[7]))
                return ash_var_find_builtin(ASH_LOGNAME);
            break;
    }
    return NULL;
}

void ash_var_set_builtin(int o, const char *v)
{
    struct ash_variable *var = ash_var_find_builtin(o);
    if(var && var->id == o)
        var->value = v;
}

const char *ash_var_get_value(struct ash_variable *var)
{
    return var == NULL? NULL: var->value;
}
