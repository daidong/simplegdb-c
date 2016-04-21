#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

struct config get_config(){
    struct config configstruct;
    FILE *file = fopen (CONFIG_FILE, "r");

    if (file != NULL){
        char line[MAXBUF];
        int i = 0;

        while (fgets(line, sizeof(line), file) != NULL){
            char *cfline;
            cfline = strstr((char *)line, DELIM);
            cfline = cfline + strlen(DELIM);
            cfline = strtok(cfline, "\n");
            if (i == 0)
                memcpy(configstruct.server_list, cfline, strlen(cfline));
            else if (i == 1)
                memcpy(configstruct.port,cfline,strlen(cfline));
            else if (i == 2)
                memcpy(configstruct.db_file,cfline,strlen(cfline));
            else if (i == 3)
                memcpy(configstruct.db_env,cfline,strlen(cfline));
            i++;
        }
        fclose(file);
    }
    return configstruct;
}
