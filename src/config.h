#ifndef _CONFIG_H_

#define CONFIG_FILE "config.conf"
#define MAXBUF 1024
#define DELIM "="

struct config{
    char server_list[MAXBUF];
    char port[MAXBUF];
    char db_file[MAXBUF];
    char db_env[MAXBUF];
};

struct config get_config();

#endif
