#ifndef RPC_H
#define RPC_H

#include "rpc_engine.h"

hg_id_t my_rpc_register(void);

MERCURY_GEN_PROC(my_rpc_out_t, ((int32_t)(ret)))

MERCURY_GEN_PROC(my_rpc_in_t,
    ((int32_t)(input_val))\
    ((hg_bulk_t)(bulk_handle)))

#endif