all: sgdb

sgdb: sgdb.c dbkey.c store.c zmqloop.c config.c
	gcc -Wall -O2 -o sgdb sgdb.c dbkey.c store.c zmqloop.c config.c -ldb -lzmq -g

clean:
	rm -f sgdb
