all: sgdb

sgdb: sgdb.c dbkey.c store.c zmqloop.c config.c
	gcc -Wall -O2 -o sgdb sgdb.c dbkey.c store.c zmqloop.c config.c -L/usr/local/opt/berkeley-db4/lib -I/usr/local/opt/berkeley-db4/include -ldb -lzmq -g

clean:
	rm -f sgdb
