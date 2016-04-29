#include "zmqloop.h"
#include "string.h"

void *context = NULL;
void *server = NULL;

void SGDB_free(void *data, void *hint){
    free(data);
}

/* @TODO: this is not correct. */
void SGDB_send_with_retry(void *s, void *data, size_t size, int REQUEST_RETRIES){

    int sequence = 0;
    int retries_left = REQUEST_RETRIES;
    while( retries_left ){
        SGDB(s, data, size);
        int expect_reply = 1;
        while (expect_reply){
            zmq_pollitem_t items[] = { {s, 0, ZMQ_POLLIN, 0}};
            int rc = zmq_poll(items, 1, REQUEST_TIMEOUT);
            if (rc == -1)
                break;
            if (items[0].revents & ZMQ_POLLIN){
                char reply[256];
                int size = zmq_recv(s, reply, 255, 0);
                if (size < 0) break;
            } else if (--retries_left == 0){
                printf("E: server not response\n");
                break;
            } else {
                printf ("W: no response from server, retrying...\n");
                zmq_close(s);
                client = zmq_socket(s, ZMQ_REQ);
                zmq_bind(s, addr);
                SGDB(s, data, size);
            }
        }
    }
}

void SGDB_send(void *s, void *data, size_t size){
    int loops = size / MAX_ZMQ_PACKAGE_SIZE + (size % MAX_ZMQ_PACKAGE_SIZE == 0) ? 0 : 1;

    while (loops--){
        zmq_msg_t msg;
        void *hint = NULL;

        if (loops == 0){ /* This is last package */
            zmq_msg_init_data (&msg, data, size, SGDB_free, hint);
            zmq_send (s, &msg, 0);
        } else {
            zmq_msg_init_data(&msg, data, MAX_ZMQ_PACKAGE_SIZE, SGDB_free, hint);
            zmq_send (s, &msg, ZMQ_SNDMORE);
            data += MAX_ZMQ_PACKAGE_SIZE;
            size -= MAX_ZMQ_PACKAGE_SIZE;
        }
        zmq_msg_close(msg);
    }
}

void SGDB_srv_loop(){
    while (1){
        int64_t more = 0;
        size_t more_size = sizeof(more);
        void *data = NULL;
        size_t size = 0;
        size_t multi = 0;
        zmq_msg_t part;

        do {
            /* Create an empty ØMQ message to hold the message part */
            int rc = zmq_msg_init (&part);
            assert (rc == 0);
            /* Block until a message is available to be received from socket */
            rc = zmq_recv (server, &part, 0);
            assert (rc == 0);

            /* Determine if more message parts are to follow */
            rc = zmq_getsockopt (server, ZMQ_RCVMORE, &more, &more_size);
            if (rc == 0){ /* This is single part message; zero-copy */
                data = zmq_msg_data(&part);
                size = zmq_msg_size(&part);
            } else { /* This is a multi-part message; Copy it to data*/
                /* @TODO: make sure have error processing */
                data = realloc(data, sizeof(char) * zmq_msg_size(&part));
                memcpy(data, zmq_msg_data(&part), zmq_msg_size(&part));
                size += zmq_msg_size(&part);
                multi = 1;
                zmq_msg_close (&part);
            }
        } while (more);

        SGDB_Command *c = (SGDB_Command *) data;

        switch (c->type) {
            case 'p':  /* insert key-value */
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
                break;
        }

        if (multi == 1){
            /* we copy message out of zmq_msg_t, we need to free it manually*/
            free(data);
        } else {
            zmq_msg_close(&part);
        }
    }
}

void SGDB_init_srv(char* _port){
    context = zmq_ctx_new();
    server = zmq_socket(context, ZMQ_REP);

    char *addr = "tcp://*:";
    addr = strcat(addr, _port);
    zmq_bind(server, "tcp://*:5555");

    //this should not exit
    SGDB_srv_loop();

    zmq_close(server);
    zmq_ctx_destroy(context);
    return;
}

void SGDB_close_clt(SGDB_RPC_Client* client){
    zmq_close(client->socket);
    zmq_ctx_destroy(client->context);
}

SGDB_RPC_Client* SGDB_init_clt(char* _addr, char* _port){
    int port = atoi(_port);
    char * addr = strcat(_addr, ":");
    addr = strcat(addr, _port);

    SGDB_RPC_Client* client = (SGDB_RPC_Client *) malloc (sizeof(SGDB_RPC_Client));

    client->context = zmq_ctx_new();
    client->socket = zmq_socket(client->context, ZMQ_REQ);
    zmq_bind(client->socket, addr);

    return client;
}
