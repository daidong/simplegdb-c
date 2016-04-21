all: sgdb
	
sgdb: sgdb.c dbkey.c store.c
	gcc -Wall -O2 -o sgdb sgdb.c dbkey.c store.c -ldb -g

clean:
	rm -f sgdb