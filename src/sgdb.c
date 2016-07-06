#include "store.h"
#include "config.h"
#include "rpc.h"
#include "rpc_engine.h"

int main(int argc, char **argv) {
	sgdb_config* conf = get_config();

	sgdb_init(conf->db_file, conf->db_env);
	
	hg_engine_init(NA_TRUE, "tcp://localhost:1234");

	/* register RPC */
	my_rpc_register();

	/* wait for shutdown */
	while (1)
		sleep(1);

	hg_engine_finalize();
	return (0);

}
