#include "store.h"
#include "config.h"
#include "zmqloop.h"


int main(int argc, char **argv) {
	struct config conf = get_config();
	SGDB_init(conf.db_file, conf.db_env);
	SGDB_init_srv(conf.port);
	SGDB_close();
}
