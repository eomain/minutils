/* Copyright 2018 eomain - this program is licensed under the 2-clause BSD license
   see LICENSE for the full license info
*/

#ifndef ASH_ENV
#define ASH_ENV

extern void ash_prompt(void);
extern void ash_env_init(void);
extern size_t ash_env_get_pwd_max(void);
extern void ash_env_pwd(void);
extern void ash_env_dir(void);
extern const char *ash_env_get_pwd(void);
extern const char *ash_env_get_dir(void);
extern const char *ash_env_get_home(void);
extern const char *ash_env_get_uname(void);
extern const char *ash_env_get_host(void);
extern const char *ash_env_get_path(void);

#endif
