# testRedisSource
test redis source，测试阅读redis源码时遇到的各种关键代码

1.测试Redis生成level随机函数
make testZslRandomLevel

测试示例：
./testZslRandomLevel 10000

2.测试Redis生成Hash
make testHashFunc

测试示例：
./testHashFunc 1

3.测试Redis zskiplist（跳跃表）插入并画出结构图
make testZslInsert

测试示例：
./testZslInsert 1.0 a 0.5 b 2.0 c
