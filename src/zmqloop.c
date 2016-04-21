#include "zmqloop.h"

void SGDB_init_srv(){
    void *context = zmq_ctx_new();
    void *server = zmq_socket(context, ZMQ_REP);
    zmq_bind(server, "tcp://*:5555");

    while (1){
        char buffer[256];
        zmq_recv(server, buffer, 255, 0);
        zmq_send(server, buffer, 255, 0);
    }

    zmq_close(server);
    zmq_ctx_destroy(context);
    return;
}

void SGDB_init_clt(){
    void *ctx = zmq_ctx_new();
    void *client = zmq_socket(ctx, ZMQ_REQ);
    zmq_bind(client, SERVER_ENDPOINT);

    int sequence = 0;
    int retries_left = REQUEST_RETRIES;
    while( retries_left ){ // && !zctx_interrupted
        char request[10];
        sprintf(request, "%d", ++sequence);
        zmq_send(client, request, 10, 0);

        int expect_reply = 1;
        while (expect_reply){
            zmq_pollitem_t items[] = { {client, 0, ZMQ_POLLIN, 0}};
            int rc = zmq_poll(items, 1, REQUEST_TIMEOUT);
            if (rc == -1)
                break;
            if (items[0].revents & ZMQ_POLLIN){
                char reply[256];
                int size = zmq_recv(client, reply, 255, 0);
                if (size < 0) break;
            } else if (--retries_left == 0){
                printf("E: server not response\n");
                break;
            } else {
                printf ("W: no response from server, retrying...\n");
                zmq_close(client);
                client = zmq_socket(ctx, ZMQ_REQ);
                zmq_bind(client, SERVER_ENDPOINT);
                zmq_send(client, request, 10, 0);
            }
        }
    }
    zmq_ctx_destroy(ctx);
    return;
}
