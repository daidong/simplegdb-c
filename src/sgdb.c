#include "store.h"
#include "config.h"
#include "zmqloop.h"

void server_loop(){
	return;
}

int main(int argc, char **argv) {
	struct config conf = get_config();
	SGDB_init(conf.db_file, conf.db_env);
	server_loop();
	SGDB_close();
}
