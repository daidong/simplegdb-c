#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libconfig.h>

#include "config.h"

SGDB_CONFIG* get_config(){

    SGDB_CONFIG* configstruct = malloc(sizeof(SGDB_CONFIG));
    
    config_t cfg;
    config_setting_t *setting;
    const char *str;

    config_init(&cfg);

    /* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(&cfg, "config.conf"))
    {
      fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
              config_error_line(&cfg), config_error_text(&cfg));
      config_destroy(&cfg);
      return NULL;
    }

    /* Get the db file name. */
    if(config_lookup_string(&cfg, "db_file", &str))
      //printf("db_file name: %s\n\n", str);
		memcpy(configstruct->db_file, str, strlen(str));
    else
      fprintf(stderr, "No 'db_file' setting in configuration file.\n");
	
    /* Get the db env dir name. */
    if(config_lookup_string(&cfg, "db_env_dir", &str))
		memcpy(configstruct->db_env, str, strlen(str));
    else
      fprintf(stderr, "No 'db_env_dir' setting in configuration file.\n");

    /* Get the db env dir name. */
    if(config_lookup_string(&cfg, "local_port", &str))
		memcpy(configstruct->port, str, strlen(str));
    else
      fprintf(stderr, "No 'local_port' setting in configuration file.\n");
    
	/* Output a list of all books in the inventory. */
    setting = config_lookup(&cfg, "servers");
    if(setting != NULL)
    {
      int count = config_setting_length(setting);
	  configstruct->server_number = count;
      int i;
      for(i = 0; i < count; ++i)
      {
        config_setting_t *server = config_setting_get_elem(setting, i);

        /* Only output the record if all of the expected fields are present. */
        const char *protocol, *address, *port;

        if(!(config_setting_lookup_string(server, "protocol", &protocol)
             && config_setting_lookup_string(server, "address", &address)
             && config_setting_lookup_string(server, "port", &port)
             ))
          continue;
		
		char addr[MAXSRVSTR] = {0};
		strcpy(addr, protocol);
        strcat(addr, "://");
		strcat(addr, address);
		strcat(addr, ":");
		strcat(addr, port);
		memcpy(configstruct->server_list[i], addr, strlen(addr));
      }
	  
	  //for (i = 0; i < configstruct->server_number; i++)
      //	printf("config: file: %s, servers: %s\n", configstruct->db_file, configstruct->server_list[i]);
    }

    config_destroy(&cfg);
    return configstruct;
}
