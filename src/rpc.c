#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#include "rpc.h"

static hg_return_t my_rpc_handler(hg_handle_t handle);
static hg_return_t my_rpc_handler_bulk_cb(const struct hg_cb_info *info);
static void my_rpc_handler_write_cb(union sigval sig);

/* struct used to carry state of overall operation across callbacks */
struct my_rpc_state
{
    hg_size_t size;
    void *buffer;
    hg_bulk_t bulk_handle;
    hg_handle_t handle;
    struct aiocb acb;
    my_rpc_in_t in;
};

hg_id_t my_rpc_register(void)
{
	hg_class_t* hg_class;
	hg_id_t tmp;

	hg_class = hg_engine_get_class();

	tmp = MERCURY_REGISTER(hg_class, "myrpc", my_rpc_in_t, my_rpc_out_t, my_rpc_handler);

	return tmp;
}

/* callback/handler triggered upon receipt of rpc request */
static hg_return_t my_rpc_handler(hg_handle_t handle)
{
	int ret;
    struct my_rpc_state *my_rpc_state_p;
    struct hg_info *hgi;

    /* set up state structure */
    my_rpc_state_p = malloc(sizeof(*my_rpc_state_p));
    assert(my_rpc_state_p);
    my_rpc_state_p->size = 512;
    my_rpc_state_p->handle = handle;
    /* This includes allocating a target buffer for bulk transfer */
    my_rpc_state_p->buffer = calloc(1, 512);
    assert(my_rpc_state_p->buffer);

    /* decode input */
    ret = HG_Get_input(handle, &my_rpc_state_p->in);
    assert(ret == HG_SUCCESS);

    printf("Got RPC request with input_val: %d\n", my_rpc_state_p->in.input_val);

    /* register local target buffer for bulk access */
    hgi = HG_Get_info(handle);
    assert(hgi);
    ret = HG_Bulk_create(hgi->hg_class, 1, &my_rpc_state_p->buffer,
        &my_rpc_state_p->size, HG_BULK_WRITE_ONLY, &my_rpc_state_p->bulk_handle);
    assert(ret == 0);

    /* initiate bulk transfer from client to server */
    ret = HG_Bulk_transfer(hgi->context, my_rpc_handler_bulk_cb,
        my_rpc_state_p, HG_BULK_PULL, hgi->addr, my_rpc_state_p->in.bulk_handle, 0,
        my_rpc_state_p->bulk_handle, 0, my_rpc_state_p->size, HG_OP_ID_IGNORE);
    assert(ret == 0);

    return(0);
}

/* callback triggered upon completion of bulk transfer */
static hg_return_t my_rpc_handler_bulk_cb(const struct hg_cb_info *info)
{
    struct my_rpc_state *my_rpc_state_p = info->arg;
    int ret;
    char filename[256];

    assert(info->ret == 0);

    /* open file (NOTE: this is blocking for now, for simplicity ) */
    sprintf(filename, "/tmp/hg-stock-%d.txt", my_rpc_state_p->in.input_val);
    memset(&my_rpc_state_p->acb, 0, sizeof(my_rpc_state_p->acb));
    my_rpc_state_p->acb.aio_fildes = 
        open(filename, O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR);
    assert(my_rpc_state_p->acb.aio_fildes > -1);

    /* set up async I/O operation (write the bulk data that we just pulled 
     * from the client)
     */ 
    my_rpc_state_p->acb.aio_offset = 0;
    my_rpc_state_p->acb.aio_buf = my_rpc_state_p->buffer;
    my_rpc_state_p->acb.aio_nbytes = 512;
    my_rpc_state_p->acb.aio_sigevent.sigev_notify = SIGEV_THREAD;
    my_rpc_state_p->acb.aio_sigevent.sigev_notify_attributes = NULL;
    my_rpc_state_p->acb.aio_sigevent.sigev_notify_function = my_rpc_handler_write_cb;
    my_rpc_state_p->acb.aio_sigevent.sigev_value.sival_ptr = my_rpc_state_p;

    /* post async write (just dump data to stdout) */
    ret = aio_write(&my_rpc_state_p->acb);
    assert(ret == 0);

    return(0);
}

/* callback triggered upon completion of async write */
static void my_rpc_handler_write_cb(union sigval sig)
{
    struct my_rpc_state *my_rpc_state_p = sig.sival_ptr;
    int ret;
    my_rpc_out_t out;

    ret = aio_error(&my_rpc_state_p->acb);
    assert(ret == 0);
    out.ret = 0;

    /* NOTE: really this should be nonblocking */
    close(my_rpc_state_p->acb.aio_fildes);

    /* send ack to client */
    /* NOTE: don't bother specifying a callback here for completion of sending
     * response.  This is just a best effort response.
     */
    ret = HG_Respond(my_rpc_state_p->handle, NULL, NULL, &out);
    assert(ret == HG_SUCCESS);
    
    HG_Bulk_free(my_rpc_state_p->bulk_handle);
    HG_Destroy(my_rpc_state_p->handle);
    free(my_rpc_state_p->buffer);
    free(my_rpc_state_p);

    return;
}