#include <stdio.h>
#include <stdlib.h>

#define ZSKIPLIST_P 0.25
#define ZSKIPLIST_MAXLEVEL 32
int statis[ZSKIPLIST_MAXLEVEL];
int zslRandomLevel(void);

int main(int argc, char**argv){
	if(argc != 2) return 1;
	
	int t;
	int testTimes = atoi(argv[1]);
	for(int i = 0; i < testTimes; i++){
		t = zslRandomLevel();
		printf("%d\n", t);
		statis[t - 1] = statis[t - 1] + 1;
	}
	char *offsetStr;
	for(int i = ZSKIPLIST_MAXLEVEL; i > 0; i--){
		offsetStr = i < (10 - 1) ? "0" : "";
		printf("level%s%d %d\n", offsetStr, i, statis[i - 1]);
	}

	return 0;
}

int zslRandomLevel(void) {
    int level = 1;

    while ((random()&0xFFFF) < (ZSKIPLIST_P * 0xFFFF))
        level += 1;

    return (level<ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
}
