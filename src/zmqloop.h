#ifndef _ZMQLOOP_H_

#include <zmq.h>

#define REQUEST_TIMEOUT 2500 //msecs, (> 1000!)
#define SERVER_ENDPOINT "tcp://localhost:5555"

#define MAX_ZMQ_PACKAGE_SIZE 4096

typedef struct _sgdb_command{
    char type;
    char *payload;
} SGDB_Command;

typedef struct _zmq_ctx_socket_{
    void *context;
    void *socket;
} SGDB_RPC_Client;

void SGDB_init_srv(char* _port);

SGDB_RPC_Client* SGDB_init_clt(char* addr);
void SGDB_close_clt(SGDB_RPC_Client* client);

void SGDB_send(void *s, void *data, size_t size);

#endif
