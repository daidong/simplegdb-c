#ifndef _CONFIG_H_

#define CONFIG_FILE "config.conf"
#define MAXPATH 1024
#define MAXSRVSTR 32
#define MAXSRV 1024
#define DELIM "="

typedef struct _config_{
    char server_list[MAXSRV][MAXSRVSTR];
	int server_number;
	char port[MAXSRVSTR];
	
    char db_file[MAXPATH];
    char db_env[MAXPATH];
} SGDB_CONFIG;

SGDB_CONFIG* get_config();

#endif
