all: sgdb

sgdb: sgdb.c dbkey.c store.c config.c sgdb_clt.c
	gcc -Wall -O2 -o sgdb sgdb.c dbkey.c store.c config.c -L/usr/local/opt/berkeley-db4/lib:/usr/local/lib -I/usr/local/opt/berkeley-db4/include:/usr/local/include -ldb -lconfig -g
	gcc -Wall -O2 -o sgdb_clt sgdb_clt.c config.c -lconfig -g

clean:
	rm -f sgdb sgdb_clt
