#include "config.h"
#include "zmqloop.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	struct config conf = get_config();
    SGDB_RPC_Client *c = SGDB_init_clt(conf.server_list, conf.port);
    SGDB_Command *m = (SGDB_Command *)malloc(sizeof(SGDB_Command));
    m->type = 'i';
    m->payload = "Insert";
    size_t size = sizeof(m->type) + strlen(m->payload);
    SGDB_send(c->socket, (void *) m, size);
    SGDB_close_clt(c);
}
