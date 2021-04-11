#include <stdlib.h>
#include <stdio.h>

#include "zmalloc.h"
#include "testZslInsert.h"


#define ZSKIPLIST_MAXLEVEL 32
#define ZSKIPLIST_P 0.25

zskiplist *zslCreate(void);
zskiplistNode *zslInsert(zskiplist *zsl, double score, robj *obj);
int zslRandomLevel(void);
zskiplistNode *zslCreateNode(int level, double score, robj *obj);

//show way
void view1(zskiplist *zsl);
void view2(zskiplist *zsl);
void viewShowOneLine(int level,zskiplistNode **levelt, char *s1, char *s2, char *s3);

int main(int argc, char**argv){
	if(argc < 3) return 1;
	
	robj *objTemp;

	zskiplist *zsl;
	int itemCount = (argc - 1) / 2;
	int inserteIndex = 0;
	//printf("total %d item need insert\n", itemCount);
	
	zsl = zslCreate();
	while(inserteIndex < itemCount){
		objTemp = zmalloc(sizeof(struct redisObject));
		objTemp->ptr = (void *)argv[(inserteIndex * 2) + 2];
		zslInsert(zsl, atof(argv[(inserteIndex * 2) + 1]), objTemp);
		//printf("insert one %d\n", inserteIndex);
		inserteIndex++;
	}
	//printf("insert success\n");	
	//view1(zsl);
	view2(zsl);


	return 0;
}

void view1(zskiplist *zsl){
	char outBuf[4096];
	outBuf[0] = '\0';
	zskiplistNode *t;

	for(int i = 0; i < ZSKIPLIST_MAXLEVEL; i++){
		//printf("level%d\n", i);
		t = zsl->header->level[i].forward;
		if(!t) continue;
		sprintf(outBuf, "     HEAD(level%d span = %d)  \n", 
				(i + 1), 
				zsl->header->level[i].span
		);
		sprintf(outBuf, "%s               ↓  \n", outBuf);
		printf(outBuf);
		while(t){
			sprintf(outBuf, "____________________________\n");
			//sprintf(outBuf, "%s| backword  = %p        \n", outBuf, t->backward);
			sprintf(outBuf, "%s| Addr  = %p        \n", outBuf, t);
			sprintf(outBuf, "%s| score = %f         \n", outBuf, t->score);
			sprintf(outBuf, "%s| value = %s \n", outBuf, (t->obj ? (char *)t->obj->ptr : ""));
			sprintf(outBuf, "%s| span  = %d                \n", outBuf, t->level[i].span);
			sprintf(outBuf, "%s￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n", outBuf);
			printf(outBuf);

			/*printf("level %d span %d score %f value %s\n", 
				i,
				t->level[i].span,
				t->score, 
				(t->obj ? (char *)t->obj->ptr : "")
			);*/
			t = t->level[i].forward;	
			if(t){
				if(i == 0){
					sprintf(outBuf, "               ↑↓  \n");
				}
				if(i != 0){
					sprintf(outBuf, "               ↓  \n");
				}
				
				//sprintf(outBuf, "%s               V  \n", outBuf);
				printf(outBuf);
			}
			if(!t){
				sprintf(outBuf, "               ↓  \n");
				sprintf(outBuf, "%s              NULL  \n\n", outBuf);
				printf(outBuf);
			}
		}
	}
}

void view2(zskiplist *zsl){
	int i;
	char outBuf[4096],lineBuf1[4096],lineBuf2[4096],lineBuf3[4096];
	outBuf[0] = '\0';
	zskiplistNode *t,*levelt[ZSKIPLIST_MAXLEVEL];
	int level = zsl->level;
	for(i = 0; i < ZSKIPLIST_MAXLEVEL; i++){
		levelt[i] = zsl->header->level[i].forward;
	}

	if(!levelt[0]) return;

	//make header
	for(i = 0; i < level; i++){
		if(i == 0) *outBuf = '\0'; 

		sprintf(outBuf, "%s     HEAD(level%d span = %d)  ",
				outBuf, 
				(i + 1), 
				zsl->header->level[i].span
		);

		if(i == (level - 1)){
			sprintf(outBuf, "%s\n", outBuf);
		}	
	}
	//print header
	printf(outBuf);
	
	viewShowOneLine(level, levelt,
				"               ↓  ",
				"                            ",
				"                            ");
	
	levelt[0] = zsl->header->level[0].forward;
	while(levelt[0]){
		viewShowOneLine(level, levelt,
				"____________________________",
				"____________________________",
				"                            ");
		viewShowOneLine(level, levelt,
				sprintf(lineBuf1, "| Addr  = %p        ", levelt[0]) ? lineBuf1 : "",
				"                             ",
				"                            ");

		viewShowOneLine(level, levelt,
				sprintf(lineBuf1, "| score = %f         ", levelt[0]->score) ? lineBuf1 : "",
				"                             ",
				"                            ");
		viewShowOneLine(level, levelt,
				sprintf(lineBuf1, "| value = %s ", (levelt[0]->obj ? (char *)levelt[0]->obj->ptr : "")) ? lineBuf1 : "",
				"                             ",
				"                            ");

		viewShowOneLine(level, levelt,
				sprintf(lineBuf1, "| span  = %d                ", levelt[0]->level[0].span) ? lineBuf1 : "",
				levelt[0] == levelt[1] ? (sprintf(lineBuf2, "    span  = %d                ", levelt[1]->level[1].span) ? lineBuf2 : "") : "",
				"                            ");
		viewShowOneLine(level, levelt,
				sprintf(lineBuf1, "￣￣￣￣￣￣￣￣￣￣￣￣￣￣") ? lineBuf1 : "",
				"￣￣￣￣￣￣￣￣￣￣￣￣￣￣",
				"                            ");

		for(int i = level - 1; i > 0; i--){
			//if level synced,do forward
			if(levelt[i] == levelt[i -1]) levelt[i] = levelt[i]->level[i].forward;
		}
		levelt[0] = levelt[0]->level[0].forward;
	
		if(levelt[0]){
			viewShowOneLine(level, levelt,
				"               ↑↓  ",
				"               ↓  ",
				"                            ");
		}

		if(!levelt[0]){
			sprintf(outBuf, "               ↓  \n");
			sprintf(outBuf, "%s              NULL  \n\n", outBuf);
			printf(outBuf);
		}
	
	}
}

void viewShowOneLine(int level,zskiplistNode **levelt, char *s1, char *s2, char *s3){
	int i;
	char outBuf[4096];
	outBuf[0] = '\0';
	//make line
	for(i = 0; i < level; i++){
		if(i == 0) *outBuf = '\0'; 

		if(i == 0){
			sprintf(outBuf, "%s%s", outBuf, s1);
		}
		if(i != 0){
			if(levelt[i] == levelt[0]){
				sprintf(outBuf, "%s%s", outBuf, s2);
			}
			if(levelt[i] != levelt[0]){
				sprintf(outBuf, "%s%s", outBuf, s3);
			}
		}

		if(i == (level - 1)){
			sprintf(outBuf, "%s\n", outBuf);
		}	
	}
	//print line
	printf(outBuf);	
}

/*
 * 创建并返回一个新的跳跃表
 *
 * T = O(1)
 */
zskiplist *zslCreate(void) {
    int j;
    zskiplist *zsl;

    // 分配空间
    zsl = zmalloc(sizeof(*zsl));

    // 设置高度和起始层数
    zsl->level = 1;
    zsl->length = 0;

    // 初始化表头节点
    // T = O(1)
    zsl->header = zslCreateNode(ZSKIPLIST_MAXLEVEL,0,NULL);
    for (j = 0; j < ZSKIPLIST_MAXLEVEL; j++) {
        zsl->header->level[j].forward = NULL;
        zsl->header->level[j].span = 0;
    }
    zsl->header->backward = NULL;

    // 设置表尾
    zsl->tail = NULL;

    return zsl;
}

/*
 * 创建一个成员为 obj ，分值为 score 的新节点，
 * 并将这个新节点插入到跳跃表 zsl 中。
 * 
 * 函数的返回值为新节点。
 *
 * T_wrost = O(N^2), T_avg = O(N log N)
 */
zskiplistNode *zslInsert(zskiplist *zsl, double score, robj *obj) {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned int rank[ZSKIPLIST_MAXLEVEL];
    int i, level;

    //redisAssert(!isnan(score));
    if(score == 0) exit(1);
    //因为后面分值如果相同这里直接对比地址的大小，所以obj不能为NULL

    // 在各个层查找节点的插入位置
    // T_wrost = O(N^2), T_avg = O(N log N)
    x = zsl->header;
    for (i = zsl->level-1; i >= 0; i--) {

        /* store rank that is crossed to reach the insert position */
        // 如果 i 不是 zsl->level-1 层
        // 那么 i 层的起始 rank 值为 i+1 层的 rank 值
        // 各个层的 rank 值一层层累积
        // 最终 rank[0] 的值加一就是新节点的前置节点的排位
        // rank[0] 会在后面成为计算 span 值和 rank 值的基础
        rank[i] = i == (zsl->level-1) ? 0 : rank[i+1];

        // 沿着前进指针遍历跳跃表
        // T_wrost = O(N^2), T_avg = O(N log N)
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                // 比对分值
                (x->level[i].forward->score == score &&
                // 比对成员， T = O(N)
                /* compareStringObjects(x->level[i].forward->obj,obj) < 0 */ 
		// 这里直接对比地址的大小
		(void *)x->level[i].forward->obj < (void *)obj))) {

            // 记录沿途跨越了多少个节点
            rank[i] += x->level[i].span;

            // 移动至下一指针
            x = x->level[i].forward;
        }
        // 记录将要和新节点相连接的节点
        update[i] = x;
    }

    /* we assume the key is not already inside, since we allow duplicated
     * scores, and the re-insertion of score and redis object should never
     * happen since the caller of zslInsert() should test in the hash table
     * if the element is already inside or not. 
     *
     * zslInsert() 的调用者会确保同分值且同成员的元素不会出现，
     * 所以这里不需要进一步进行检查，可以直接创建新元素。
     */

    // 获取一个随机值作为新节点的层数
    // T = O(N)
    level = zslRandomLevel();

    // 如果新节点的层数比表中其他节点的层数都要大
    // 那么初始化表头节点中未使用的层，并将它们记录到 update 数组中
    // 将来也指向新节点
    if (level > zsl->level) {

        // 初始化未使用层
        // T = O(1)
        for (i = zsl->level; i < level; i++) {
            rank[i] = 0;
            update[i] = zsl->header;
            update[i]->level[i].span = zsl->length;
        }

        // 更新表中节点最大层数
        zsl->level = level;
    }

    // 创建新节点
    x = zslCreateNode(level,score,obj);

    // 将前面记录的指针指向新节点，并做相应的设置
    // T = O(1)
    for (i = 0; i < level; i++) {
        
        // 设置新节点的 forward 指针
        x->level[i].forward = update[i]->level[i].forward;
        
        // 将沿途记录的各个节点的 forward 指针指向新节点
        update[i]->level[i].forward = x;

        /* update span covered by update[i] as x is inserted here */
        // 计算新节点跨越的节点数量
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);

        // 更新新节点插入之后，沿途节点的 span 值
        // 其中的 +1 计算的是新节点
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    /* increment span for untouched levels */
    // 未接触的节点的 span 值也需要增一，这些节点直接从表头指向新节点
    // T = O(1)
    for (i = level; i < zsl->level; i++) {
        update[i]->level[i].span++;
    }

    // 设置新节点的后退指针
    x->backward = (update[0] == zsl->header) ? NULL : update[0];
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        zsl->tail = x;

    // 跳跃表的节点计数增一
    zsl->length++;

    return x;
}

int zslRandomLevel(void) {
    int level = 1;

    while ((random()&0xFFFF) < (ZSKIPLIST_P * 0xFFFF))
        level += 1;

    return (level<ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
}

zskiplistNode *zslCreateNode(int level, double score, robj *obj) {
    
    // 分配空间
    zskiplistNode *zn = zmalloc(sizeof(*zn)+level*sizeof(struct zskiplistLevel));

    // 设置属性
    zn->score = score;
    zn->obj = obj;

    return zn;
}
