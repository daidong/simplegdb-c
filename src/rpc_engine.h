#ifndef RPC_ENGINE_H
#define RPC_ENGINE_H

#include <mercury_bulk.h>
#include <mercury.h>
#include <mercury_macro.h>
#include <mercury_bulk.h>

void sgdb_engine_init(na_bool_t listen, const char *local_addr);
void sgdb_engine_finalize(void);
hg_class_t* sgdb_engine_get_class(void);
void sgdb_engine_addr_lookup(const char* name, na_cb_t cb, void *arg);
void sgdb_engine_create_handle(na_addr_t addr, hg_id_t id, hg_handle_t *handle);

#endif /* RPC_ENGINE_H */
