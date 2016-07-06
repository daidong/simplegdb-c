#ifndef _STORE_H_

#include <db.h>

int sgdb_init(char *db_file, char *env_dir);
void sgdb_close();
int sgdb_insert(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts, char *val, u_int32_t vsize);
char* sgdb_get(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts, u_int32_t *vsize);
int sgdb_del(char *src, u_int32_t ssize, char *dst, u_int32_t dsize, u_int32_t type, u_int64_t ts);

#endif
