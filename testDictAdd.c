#include "dict.h"

#include <stdio.h>


unsigned int dictIntHashFunctionDup(const void *k);
void viewHt(dictht *ht, int number);

dictType testDictType = {
    dictIntHashFunctionDup,            /* hash function */
    NULL,                       /* key dup */
    NULL,                       /* val dup */
    NULL,      /* key compare */
    NULL,          /* key destructor */
    NULL          /* val destructor */
};


int main(int argc, char**argv){
	if(argc < 3) return 1;

	int itemCount = (argc - 1) / 2;
	int inserteIndex = 0;

	struct dict *d = dictCreate(&testDictType, NULL);

	while(inserteIndex < itemCount){
		dictAdd(d, (void*)argv[(inserteIndex * 2) + 1], (void*)argv[(inserteIndex * 2) + 2]);
		inserteIndex++;
	}

	viewHt(&d->ht[0], 0);
	viewHt(&d->ht[1], 1);

	return 0;
}

void viewHt(dictht *ht, int number){
	int i;
	char buf[8192];
	dictEntry *de;
	
	printf("\ndict%d size = %d, sizemask = 0X%X, used = %d\n", number,ht->size, ht->sizemask, ht->used);
	for(int i = 0; i < ht->size; i++){
		printf("slot %02d, Adder = 0X%X, using:%s ", 
			(i + 1), 
			(void *)&ht->table[i], 
			(ht->table[i] ? "true" : "false")
			);
		de = ht->table[i];
		if(de){
			buf[0] = '\0';
			while(de){
				sprintf(buf, "%s %s", buf, (char *)de->v.val);
				de = de->next;
				if(de){
					sprintf(buf, "%s =>", buf);
				}
			}
			printf("value = %s", buf);
		}
		printf("\n");
	}
}

unsigned int dictIntHashFunctionDup(const void *k)
{
    unsigned int key = *((unsigned int *)k);
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}
