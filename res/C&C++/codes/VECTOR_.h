/* 2015-12-03 */
/* std::vector源码浅析 */

std::vector是stl标准模板库中的一个序列容器，其源码实现有几个特点：
（1）C++中include的标准头文件是没有“.h”后缀的，其实这是C++的一层包裹，内部实现还是在“.h”文件中完成的，以我们include的vector为例，其中还会include一些真正的vector实现文件，如“stl_vector.h”。
（2）vector的实现有一些内部使用的函数和数据类型，它们以下划线“_”开头。
（3）vector是个模板类，继承自_Vector_base，_Vector_base是个struct，它有一个嵌套的struct为_Vector_impl，其中有三个关键的指针成员变量，_M_start，_M_finish，_M_end_of_storage，明白这三个变量对vector的理解很有帮助。
（4）我们通常使用的vector接口，大部分是通过形如“_M_xxx”的内部函数实现的。
（5）vector的操作符首先实现了判等“==”和小于“<”，其它的操作符都是基于这两个操作符实现的。
（6）vector中有一些C++11引入的语法，可在源码中茶看得到。
下面看一下源码中的“vector”和“stl_vector.h”都有什么内容。
/* vector */
/* stl_vector.h */
