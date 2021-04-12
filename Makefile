testHashFunc: testHashFunc.c
	gcc -o target/testHashFunc testHashFunc.c

testZslRandomLevel: testZslRandomLevel.c
	gcc -o testZslRandomLevel testZslRandomLevel.c

testZslInsert: testZslInsert.c
	gcc -I ./include/ -o testZslInsert testZslInsert.c ./include/zmalloc.c

testDictAdd: testDictAdd.c
	gcc -I ./include/ -D_DEFAULT_SOURCE -o testDictAdd testDictAdd.c ./include/zmalloc.c ./include/dict.c
