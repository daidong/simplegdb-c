#ifndef _ZMQLOOP_H_

#include <zmq.h>

#define REQUEST_TIMEOUT 2500 //msecs, (> 1000!)
#define REQUEST_RETRIES 3
#define SERVER_ENDPOINT "tcp://localhost:5555"

void SGDB_init_srv();
void SGDB_init_clt();

#endif
