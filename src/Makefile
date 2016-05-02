all: sgdb

sgdb: sgdb.c dbkey.c store.c zmqloop.c config.c sgdb_clt.c
	gcc -Wall -O2 -o sgdb sgdb.c dbkey.c store.c zmqloop.c config.c -L/usr/local/opt/berkeley-db4/lib -I/usr/local/opt/berkeley-db4/include -ldb -lzmq -g
	gcc -Wall -O2 -o sgdb_clt sgdb_clt.c zmqloop.c config.c -lzmq -g

clean:
	rm -f sgdb sgdb_clt
