#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "zmqloop.h"
#include "string.h"

void SGDB_send(void *s, void *data, size_t size){
    int loops = size / MAX_ZMQ_PACKAGE_SIZE + ((size % MAX_ZMQ_PACKAGE_SIZE == 0) ? 0 : 1);

    while (loops--){		
        zmq_msg_t msg;

        if (loops == 0){ /* This is last package */
            int rc = zmq_msg_init_size (&msg, size);
            assert (rc == 0);
            memcpy(zmq_msg_data (&msg), data, size);
            rc = zmq_msg_send (&msg, s, 0);
			assert (rc == size);
        } else {
            int rc = zmq_msg_init_size (&msg, size);
            assert (rc == 0);
            memcpy(zmq_msg_data (&msg), data, MAX_ZMQ_PACKAGE_SIZE);
            rc = zmq_msg_send (&msg, s, ZMQ_SNDMORE);
			assert (rc == MAX_ZMQ_PACKAGE_SIZE);
            data += MAX_ZMQ_PACKAGE_SIZE;
            size -= MAX_ZMQ_PACKAGE_SIZE;
        }
        zmq_msg_close(&msg);
    }
}

void SGDB_srv_loop(void *server){
	
    while (1){
        int64_t more = 0;
        size_t more_size = sizeof(more);
        void *data = NULL;
        size_t size = 0;
        
        do {
            /* Create an empty ØMQ message to hold the message part */
            zmq_msg_t part;
            int rc = zmq_msg_init (&part);
            assert (rc == 0);
			
            /* Block until a message is available to be received from socket */
            rc = zmq_msg_recv (&part, server, 0);
			assert (rc != -1);
			
            /* Determine if more message parts are to follow */
            rc = zmq_getsockopt (server, ZMQ_RCVMORE, &more, &more_size);
			assert (rc == 0);
			
			data = realloc(data, sizeof(char) * zmq_msg_size(&part));
            memcpy(data, zmq_msg_data(&part), zmq_msg_size(&part));
            size += zmq_msg_size(&part);
            
            zmq_msg_close (&part);
            
        } while (more);

        SGDB_Command *c = (SGDB_Command *) data;
        switch (c->type) {
            case 'i':  /* insert key-value */
                printf("Insert\n");
                SGDB_send(server, "Insert", 5);
                break;
            case 'g':
                printf("Get\n");
                SGDB_send(server, "Get", 3);
                break;
            case 's':
                printf("Scan\n");
                SGDB_send(server, "Scan", 4);
                break;
            default:
                printf("Default\n");
                SGDB_send(server, "Default", 7);
                break;
        }

        free(data);
    }
}

void SGDB_init_srv(char* _port){
    char addr[32] = "tcp://*:";
    strcat(addr, _port);
    printf("binding address: %s\n", addr);

	void* context = zmq_ctx_new();
    void* server = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(server, addr);
	assert (rc == 0);
	
    //this should not exit
    SGDB_srv_loop(server);

    zmq_close(server);
    zmq_ctx_destroy(context);
    return;
}

SGDB_RPC_Client* SGDB_init_clt(char* addr){
    printf("Connect to address: %s\n", addr);
	
    SGDB_RPC_Client* client = (SGDB_RPC_Client *) malloc (sizeof(SGDB_RPC_Client));

    client->context = zmq_ctx_new();
    client->socket = zmq_socket(client->context, ZMQ_REQ);
    zmq_connect(client->socket, addr);
	
	return client;
}


void SGDB_close_clt(SGDB_RPC_Client* client){
    zmq_close(client->socket);
    zmq_ctx_destroy(client->context);
}