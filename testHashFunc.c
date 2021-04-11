#include <stdio.h>

unsigned int dictIntHashFunction(unsigned int key);

int main(int argc, char **argv){
	if(argc != 2) return 1;
	int t = dictIntHashFunction(atoi(argv[1]));
	printf("%d\n", t);
	return 0;
}

unsigned int dictIntHashFunction(unsigned int key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}
