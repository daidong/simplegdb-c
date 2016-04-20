#ifndef _STORE_H_

#include <db.h>

/* global variables */

DB_ENV *SGDB_Env;
DB *SGDB_dbp;

const char *SGDB_DB_FILE = "db";
const char *SGDB_ENV_DIR = "/tmp/gdb";

int SGDB_init();
void SGDB_close();
int SGDB_insert(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts, char *val, u_int32_t vsize);
char* SGDB_get(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts, u_int32_t *vsize);
int SGDB_del(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts);

#endif