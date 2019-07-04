#include <stdio.h>
#include "hashmap.h"

int main(int argc,char *argv[]){
	printf("main\n");
	map_t mhashmap = hashmap_new();
	void *i = 1;
	hashmap_put(mhashmap,"i",i);
	any_t *hash_i;
	hashmap_get(mhashmap,"i",(void**)(&hash_i));

	printf("hash_i: %d",hash_i);


	return 1;
}