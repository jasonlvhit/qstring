qstring:一个受Redis中的SDS和Python中的字符串对象洗脑的动态字符串库
=======

QString库是在标准C字符串(一个以'\0'结尾的char*)类型上的一个扩展，主要解决了下面这几方面的问题：

* 在O(1)的时间内获取字符串长度
* 池化(intern)短字符串
* 提升长字符串追加操作中的效率，减少内存分配和复制次数
* 使用引用计数管理相同的字符串，避免不必要的字符串拷贝
* 支持深拷贝(deepcopy)
* 缓存字符串哈希值，更快的字符串比较操作

在QString库中，我们使用```qstring```标识符声明一个QString类型的实例，使用```qstrnew```函数从一个字符串字面值(const char*)构造一个qstring类型变量：

``` c
qstring s = qstrnew("Hello world")
```

在QString实现的背后，qstring类型其实等价于char*，所以，理论上，我们可以使用标准C中的所有适用于char*的函数来操作qstring.但是适用于普通字符串的函数会破坏一些qstring的信息，所以qstring覆盖了基本的函数操作，参见后面会补充进来的API。

正在开发，修复一些问题和实现常用字符串操作函数。

关于Redis中的SDS和Python中的字符串类型，可以参考下面的两篇博客：

* [Redis中的SDS](https://gist.github.com/jasonlvhit/bdd88571331d7b6dac1e)
* [Python中字符串对象的实现](https://gist.github.com/jasonlvhit/41977c043634eb6769b3)
