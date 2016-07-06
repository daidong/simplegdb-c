#include <assert.h>
#include <pthread.h>
#include <unist.h>

#include "rpc_engine.h"

static na_class_t *network_class = NULL;
static na_context_t *na_context = NULL;

static hg_context_t *sgdb_context = NULL;
static hg_class_t *sgdb_class = NULL;

static pthread_t sgdb_progress_tid;
static int sgdb_progress_shutdown_flag = 0;
static void* sgdb_progress_fn(void* foo);

void sgdb_engine_init(na_bool_t listen, const char* local_addr)
{
	int ret;

    /* boilerplate HG initialization steps */
    network_class = NA_Initialize(local_addr, listen);
    assert(network_class);

    na_context = NA_Context_create(network_class);
    assert(na_context);

    sgdb_class = HG_Init_na(network_class, na_context);
    assert(sgdb_class);

    sgdb_context = HG_Context_create(sgdb_class);
    assert(hg_context);

    /* start up thread to drive progress */
    ret = pthread_create(&sgdb_progress_tid, NULL, sgdb_progress_fn, NULL);
    assert(ret == 0);

    return;
}

void sgdb_engine_finalize(void)
{
	int ret;

    /* tell progress thread to wrap things up */
    sgdb_progress_shutdown_flag = 1;

    /* wait for it to shutdown cleanly */
    ret = pthread_join(sgdb_progress_tid, NULL);
    assert(ret == 0);

    return;
}

static void* sgdb_progress_fn(void* foo)
{
	hg_return_t ret;
    unsigned int actual_count;
    (void)foo;

    while(!sgdb_progress_shutdown_flag)
    {
        do {
            ret = HG_Trigger(hg_context, 0, 1, &actual_count);
        } while((ret == HG_SUCCESS) && actual_count && !sgdb_progress_shutdown_flag);

        if(!sgdb_progress_shutdown_flag)
            HG_Progress(hg_context, 100);
    }

    return(NULL);
}

hg_class_t* hg_engine_get_class(void)
{
	return (sgdb_class);
}

void hg_engine_addr_lookup(const char* name, na_cb_t cb, void *arg)
{
	na_return_t ret;

    ret = NA_Addr_lookup(network_class, na_context, cb, arg, name, NA_OP_ID_IGNORE);
    assert(ret == NA_SUCCESS);

    return;
}

void hg_engine_create_handle(na_addr_t addr, hg_id_t id, hg_handle_t* handle)
{
	hg_return_t ret;

    ret = HG_Create(hg_context, addr, id, handle);
    assert(ret == HG_SUCCESS);

    return;
}