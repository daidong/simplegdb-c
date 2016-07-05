#include <assert.h>
#include <pthread.h>
#include <unist.h>

#include "rpc_engine.h"

static na_class_t *network_class = NULL;
static na_context_t *na_context = NULL;
static hg_context_t *hg_context = NULL;
static hg_class_t *hg_class = NULL;

static pthread_t hg_progress_tid;
static int hg_progress_shutdown_flag = 0;
static void* hg_progress_fn(void* foo);

void hg_engine_init(na_bool_t listen, const char* local_addr)
{

}

void hg_engine_finalize(void)
{

}

static void* hg_progress_fn(void* foo)
{

}

hg_class_t* hg_engine_get_class(void)
{
	return (hg_class);
}

void hg_engine_addr_lookup(const char* name, na_cb_t cb, void *arg)
{

}

void hg_engine_create_handle(na_addr_t addr, hg_id_t id, hg_handle_t* handle)
{
	
}