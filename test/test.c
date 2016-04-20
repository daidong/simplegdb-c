#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _slice_{
	int size;
	char *data;
} Slice;

typedef struct _DBKey_ {
    Slice src;
	Slice dst;
	long ts;
	int type;
} DBKey;

void pprint(const DBKey* key){
	printf("DBKey: ");
	for (int i = 0; i < key->src.size; i++)
		printf("%c", *(key->src.data + i));
	printf(" -> ");
	for (int i = 0; i < key->dst.size; i++)
		printf("%c", *(key->dst.data + i));
	printf(" [%d][%ld]\n", key->type, key->ts);
}

DBKey* build(char *a){
	DBKey *key = malloc(sizeof(DBKey));
	char *p = a;
	
	memcpy(&key->src.size, p, sizeof(short));
	key->src.data = (char *) malloc(key->src.size);
	memcpy(key->src.data, p + sizeof(short), key->src.size);
	
	memcpy(&key->dst.size, p + sizeof(short) + key->src.size, sizeof(short));
	key->dst.data = (char *) malloc(key->dst.size);
	memcpy(key->dst.data, p + sizeof(short) * 2 + key->src.size, key->dst.size);
	
	memcpy(&key->ts, p + 2 * sizeof(short) + key->src.size + key->dst.size, sizeof(long));	
	memcpy(&key->type, p + 2 * sizeof(short) + key->src.size + key->dst.size + sizeof(long), sizeof(int));

	return key;
}

char *decompose(const DBKey* key, int *size){
	int total = key->src.size + sizeof(short) * 2 + key->dst.size + sizeof(int) + sizeof(long);
	* size = total;
	char *p = malloc(total);
	memcpy(p, &key->src.size, sizeof(short));
	memcpy(p + sizeof(short), key->src.data, key->src.size);
	
	memcpy(p + sizeof(short) + key->src.size, &key->dst.size, sizeof(short));
	memcpy(p + 2 * sizeof(short) + key->src.size, key->dst.data, key->dst.size);
	
	memcpy(p + 2 * sizeof(short) + key->src.size + key->dst.size, &key->ts, sizeof(long));
	memcpy(p + 2 * sizeof(short) + key->src.size + key->dst.size + sizeof(long), &key->type, sizeof(int));
	
	return p;
}


int main(){
	DBKey key;
	Slice _a = {5, "hello"};
	Slice _b = {5, "world"};
	key.src = _a;
	key.dst = _b;
	key.ts = 0;
	key.type = 1;
	pprint(&key);
	
	int size;
	char *buffer = decompose(&key, &size);
	
	DBKey* key2 = build(buffer);
	
	pprint(key2);
	
	return 0;
}