/* stl_vector.h */

//>头文件普遍都有的宏保护
#ifndef _STL_VECTOR_H
#define _STL_VECTOR_H 1

//>Iterator迭代器通用的函数, 如distance/advance/next/prev
#include <bits/stl_iterator_base_funcs.h>

//>异常控制的几个函数声明
#include <bits/functexcept.h>

//>一些精巧的宏定义,typedef了一些数据类型
#include <bits/concept_check.h>

//>__cplusplus C++版本
#if __cplusplus >= 201103L
//>initializer_list模板类
//>C++11引入的, 初始化列表, 提供了迭代器
#include <initializer_list>
#endif

//>_GLIBCXX_VISIBILIT在c++config.h中有定义
//>简单来说即__attribute__((__visibility__("default")))
//>这个是gcc __attribute__的用法
//>详细定义如下
/***********
#define _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY 1

#if _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
# define _GLIBCXX_VISIBILITY(V) __attribute__((__visibility__(#V)))
#else
# define _GLIBCXX_VISIBILITY(V) _GLIBCXX_PSEUDO_VISIBILITY(V)
#endif

#ifndef _GLIBCXX_PSEUDO_VISIBILITY
# define _GLIBCXX_PSEUDO_VISIBILITY(V)
#endif
**********/
namespace std _GLIBCXX_VISIBILITY(default)
{
//>_GLIBCXX_BEGIN_NAMESPACE_CONTAINER在c++config.h中有定义
//>这里即一个空的宏定义
//>详细定义如下
/**********
#if defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_PROFILE)
# define _GLIBCXX_STD_C __cxx1998
# define _GLIBCXX_BEGIN_NAMESPACE_CONTAINER \
	namespace _GLIBCXX_STD_C { _GLIBCXX_BEGIN_NAMESPACE_VERSION
# define _GLIBCXX_END_NAMESPACE_CONTAINER \
	} _GLIBCXX_END_NAMESPACE_VERSION
#endif

#ifndef _GLIBCXX_STD_C
# define _GLIBCXX_STD_C std
#endif

#ifndef _GLIBCXX_BEGIN_NAMESPACE_CONTAINER
# define _GLIBCXX_BEGIN_NAMESPACE_CONTAINER
#endif

#ifndef _GLIBCXX_END_NAMESPACE_CONTAINER
# define _GLIBCXX_END_NAMESPACE_CONTAINER
#endif

#define _GLIBCXX_INLINE_VERSION 0 

#if _GLIBCXX_INLINE_VERSION
# define _GLIBCXX_BEGIN_NAMESPACE_VERSION namespace __7 {
# define _GLIBCXX_END_NAMESPACE_VERSION }
#else
# define _GLIBCXX_BEGIN_NAMESPACE_VERSION
# define _GLIBCXX_END_NAMESPACE_VERSION
#endif
**********/
_GLIBCXX_BEGIN_NAMESPACE_CONTAINER

  //>_Vector_base为vector的基类, 模板类
  //>_Tp是个具体的类型
  //>_Alloc为内存分配器, 可参照std::allocator
  template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
      //>typedef定义类型的别名
      //>typename表示其后的内容是个类型, 否则会被当作普通字段
      //>__gnu_cxx名字空间中的__alloc_traits是allocator的统一接口
      //>_Tp_alloc_type定义的简单形式如下
      //>typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
      //>如果模板参数_Alloc为std::allocator的话, _Tp_alloc_type的定义如下
      //>typedef typename allocator<_Tp> _Tp_alloc_type;
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
        rebind<_Tp>::other _Tp_alloc_type;
      //>同上, pointer即typedef typename _Tp_alloc_type::pointer pointer;
      //>同上, 如果模板参数_Alloc为std::allocator的话, pointer将非常简单, 如下所示
      //>typedef typename _Tp* pointer;
      //>绕了一大圈, 其实很简单！
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;

      //>嵌套struct
      //>_Vector_base implementation
      struct _Vector_impl 
      : public _Tp_alloc_type
      {
        //>三个重要的指针成员变量, 贯穿于整个vector
        //>用集合的方法表示vector就是[_M_start, _M_finish)
        //>指向vector中第一个元素的地址
        pointer _M_start;
        //>指向vector中最后一个元素的下一个位置的地址
        pointer _M_finish;
        //>指向vector的容量的末地址
        pointer _M_end_of_storage;

        //>构造函数的两个版本
        //>初始化列表中调用了对应的基类构造函数
        //>初始化列表中初始化了各指针成员变量, 且初始化顺序与变量声明的顺序保持一致
        _Vector_impl()
        : _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0)
        { }

        _Vector_impl(_Tp_alloc_type const& __a)
        : _Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
        { }

#if __cplusplus >= 201103L
        //>符号&&即rvalue, 右值引用, C++11引进的, 调用std::move
        //>符号&即lvalue, 传统的左值引用
        _Vector_impl(_Tp_alloc_type&& __a)
        : _Tp_alloc_type(std::move(__a)),
        _M_start(0), _M_finish(0), _M_end_of_storage(0)
        { }
#endif

        //>数据交换, 参数类型为&, 直接调用了std::swap
        //>交换的是三个指针成员变量
        void _M_swap_data(_Vector_impl& __x)
        {
          std::swap(_M_start, __x._M_start);
          std::swap(_M_finish, __x._M_finish);
          std::swap(_M_end_of_storage, __x._M_end_of_storage);
        }
      }; //> struct _Vector_impl
      
    public:
      //>stl中多处使用了typedef, 目的就是让数据类型更加简单、具体
      typedef _Alloc allocator_type;

      //>非const版本与const版本的_M_get_Tp_allocator
      //>使用static_cast把子类指针转化为父类指针, 即up-cast, 类型安全
      //>_GLIBCXX_NOEXCEPT即异常相关的宏定义
      /**********
      #ifndef _GLIBCXX_NOEXCEPT
      # if __cplusplus >= 201103L
      #  define _GLIBCXX_NOEXCEPT noexcept
      # else
      #  define _GLIBCXX_NOEXCEPT
      # endif
      #endif
      **********/
      //>noexcept是C++11引进的, 即不抛出任何异常, 等价于throw()
      _Tp_alloc_type&
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
      { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); }

      const _Tp_alloc_type&
      _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT
      { return *static_cast<const _Tp_alloc_type*>(&this->_M_impl); }

      //>获取allocator_type对象
      //>可理解为_Alloc<_Tp>类型的对象
      //>调用了某个版本的构造函数
      allocator_type
      get_allocator() const _GLIBCXX_NOEXCEPT
      { return allocator_type(_M_get_Tp_allocator()); }

      //>下面是几个版本的构造函数
      _Vector_base()
      : _M_impl() { }

      _Vector_base(const allocator_type& __a)
      : _M_impl(__a) { }

      //>参数__n指定了vector大小
      //>调用_M_create_storage分配内存
      _Vector_base(size_t __n)
      : _M_impl()
      { _M_create_storage(__n); }

      _Vector_base(size_t __n, const allocator_type& __a)
      : _M_impl(__a)
      { _M_create_storage(__n); }

#if __cplusplus >= 201103L
      //>rvalue版本的几个构造函数
      _Vector_base(_Tp_alloc_type&& __a)
      : _M_impl(std::move(__a)) { }

      _Vector_base(_Vector_base&& __x)
      : _M_impl(std::move(__x._M_get_Tp_allocator()))
      { this->_M_impl._M_swap_data(__x._M_impl); }

      _Vector_base(_Vector_base&& __x, const allocator_type& __a)
      : _M_impl(__a)
      {
        if (__x.get_allocator() == __a)
          this->_M_impl._M_swap_data(__x._M_impl);
        else
        {
          size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start;
          _M_create_storage(__n);
        }
      }
#endif

      //>析构, 内存释放, 最终会调用到::operator delete操作符
      ~_Vector_base()
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
		      - this->_M_impl._M_start); }

    public:
      //>成员变量类型为内嵌的struct, 一种简单的设计模式
      _Vector_impl _M_impl;

      //>内存分配, 最终会调用到::operator new操作符
      pointer
      _M_allocate(size_t __n)
      { return __n != 0 ? _M_impl.allocate(__n) : 0; }

      //>内存释放
      void
      _M_deallocate(pointer __p, size_t __n)
      {
        if (__p)
          _M_impl.deallocate(__p, __n);
      }

    private:
      //>私有的创建存储空间的函数
      //>调用上面的_M_Allocate来分配内存
      void
      _M_create_storage(size_t __n)
      {
        this->_M_impl._M_start = this->_M_allocate(__n);
        this->_M_impl._M_finish = this->_M_impl._M_start;
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
      }
    };


  //>真正的vector模板类, _Alloc默认类型为std::vector<_Tp>
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    //>protected方式继承自_Vector_base, 继承下来的public内容会将级至protected
    //>_Vector_base为struct, 默认的访问级别为public
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
      //>_Alloc_value_type即_Tp
      typedef typename _Alloc::value_type _Alloc_value_type;
      //>concept_check.h中的宏, 作类型检查, 从最后一个参数可以看出对类型作何检查
      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)
      
      //>_Base为vector基类的类型
      typedef _Vector_base<_Tp, _Alloc> _Base;
      //>_Tp_alloc_type可理解为_Alloc<_Tp>
      typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

    public:
      //>typedef常用的数据类型
      //>_Tp
      typedef _Tp value_type;
      //>Tp*
      typedef typename _Base::pointer pointer;
      //>const Tp*
      typedef typename _Alloc_traits::const_pointer const_pointer;
      //>_Tp&
      typedef typename _Alloc_traits::reference reference;
      //>const _Tp&
      typedef typename _Alloc_traits::const_reference const_reference;
      //>iterator
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      //>const_iterator
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector> const_iterator;
      //>const_reverse_iterator
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      //>reverse_iterator
      typedef std::reverse_iterator<iterator> reverse_iterator;
      /**********
      namespace std
      {
        typedef __SIZE_TYPE__ size_t;
        typedef __PTRDIFF_TYPE__ ptrdiff_t;
      }
      **********/
      typedef size_t size_type;
      typedef std::ptrdiff_t difference_type;
      //>_Alloc
      typedef _Alloc allocator_type;

    protected:
      //>using常用于namespapce
      //>这个地方的using使用了基类的成员, 甚至可以使用基类的private成员
      //>C++11还引进了using的其它用法, 类似于typedef
      using _Base::_M_allocate;
      using _Base::_M_deallocate;
      using _Base::_M_impl;
      using _Base::_M_get_Tp_allocator;

    public:
      //>默认构造函数, vector为空
      vector()
      : _Base() { }

      //>指定_Alloc的构造函数, vector也为空
      //>explicit关键字, 构造函数可以传递一个参数时的惯用法, 防止类型隐式转换
      explicit
      vector(const allocator_type& __a)
      : _Base(__a) { }

#if __cplusplus >= 201103L
      //>构造函数, 指定了vector大小和默认的内存分配器
      //>vector采取了默认的初始化方式, 调用_M_default_initialize
      explicit
      vector(size_type __n, const allocator_type& __a = allocator_type())
      : _Base(__n, __a)
      { _M_default_initialize(__n); }

      //>构造函数, 指定了vector大小、元素类型和默认的内存分配器
      //>通过指定的value_type初始化vector, 调用_M_fill_initialize
      vector(size_type __n, const value_type& __value,
	     const allocator_type& __a = allocator_type())
      : _Base(__n, __a)
      { _M_fill_initialize(__n, __value); }
#else
      //>构造函数, 指定了vector大小、默认的元素类型和默认的内存分配器
      //>同样, 调用_M_fill_initialize进行初始化
      explicit
      vector(size_type __n, const value_type& __value = value_type(),
	     const allocator_type& __a = allocator_type())
      : _Base(__n, __a)
      { _M_fill_initialize(__n, __value); }
#endif

      //>拷贝构造函数
      //>拷贝了同样的元素和内存分配器
      vector(const vector& __x)
      : _Base(__x.size(),
        _Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator()))
      {
        this->_M_impl._M_finish =
          std::__uninitialized_copy_a(__x.begin(), __x.end(),
          this->_M_impl._M_start, _M_get_Tp_allocator());
      }

#if __cplusplus >= 201103L
      //>rvalue版本的构造函数
      //>noexcept不抛出任何异常
      //>调用了std::move
      vector(vector&& __x) noexcept
      : _Base(std::move(__x)) { }

      //>指定了内存分配器的copy constructor
      //>调用了std::__uninitialized_copy_a
      vector(const vector& __x, const allocator_type& __a)
      : _Base(__x.size(), __a)
      {
        this->_M_impl._M_finish =
          std::__uninitialized_copy_a(__x.begin(), __x.end(),
          this->_M_impl._M_start, _M_get_Tp_allocator());
      }

      //>指定了内存分配器的move constructor
      //>当内存分配器不同时才进行真正的拷贝
      vector(vector&& __rv, const allocator_type& __m)
      : _Base(std::move(__rv), __m)
      {
        if (__rv.get_allocator() != __m)
        {
          this->_M_impl._M_finish =
            std::__uninitialized_move_a(__rv.begin(), __rv.end(),
              this->_M_impl._M_start, _M_get_Tp_allocator());
          __rv.clear();
        }
      }

      //>初始化列表形式的构造函数, 会调用value_type类型的拷贝构造函数
      //>调用了_M_range_initialize
      vector(initializer_list<value_type> __l,
	     const allocator_type& __a = allocator_type())
      : _Base(__a)
      {
        _M_range_initialize(__l.begin(), __l.end(),
          random_access_iterator_tag());
      }
#endif

#if __cplusplus >= 201103L
      //>函数参数为迭代器的模板构造函数, 以及默认的内存分配器
      //>调用了_M_initialize_dispatch进行初始化
      template<typename _InputIterator,
        typename = std::_RequireInputIter<_InputIterator> >
        vector(_InputIterator __first, _InputIterator __last,
          const allocator_type& __a = allocator_type())
        : _Base(__a)
        { _M_initialize_dispatch(__first, __last, __false_type()); }
#else
      template<typename _InputIterator>
        vector(_InputIterator __first, _InputIterator __last,
          const allocator_type& __a = allocator_type())
        : _Base(__a)
        {
          typedef typename std::__is_integer<_InputIterator>::__type _Integral;
          _M_initialize_dispatch(__first, __last, _Integral());
        }
#endif

      //>析构函数, 清空vector中的元素
      //>需要注意的是, 如果vector中的元素为指针的话, 指针指向的内存需要我们自己管理
      ~vector() _GLIBCXX_NOEXCEPT
      { std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
		      _M_get_Tp_allocator()); }

      //>赋值操作符
      vector&
      operator=(const vector& __x);

#if __cplusplus >= 201103L
      //>rvalue版本的赋值操作符
      //>constexpr是C++11引入的, 常量表达式
      //>调用了_M_move_assign
      vector&
      operator=(vector&& __x) noexcept(_Alloc_traits::_S_nothrow_move())
      {
        constexpr bool __move_storage = 
           _Alloc_traits::_S_propagate_on_move_assign()
           || _Alloc_traits::_S_always_equal();
        _M_move_assign(std::move(__x), integral_constant<bool, __move_storage>());
        return *this;
      }

      //>参数为initializer_list的赋值操作符
      //>直接调用assign进行赋值
      vector&
      operator=(initializer_list<value_type> __l)
      {
        this->assign(__l.begin(), __l.end());
        return *this;
      }
#endif

      //>下面是几个版本的赋值函数assign
      //>新的数据会覆盖vector中原来的数据
      //>指定了vector的长度和元素类型, 调用了_M_fill_assign
      void
      assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }

#if __cplusplus >= 201103L
      //>assign参数为迭代器的模板函数
      //>调用_M_assign_dispatch
      template<typename _InputIterator,
          typename = std::_RequireInputIter<_InputIterator>>
        void
        assign(_InputIterator __first, _InputIterator __last)
        { _M_assign_dispatch(__first, __last, __false_type()); }
#else
      template<typename _InputIterator>
        void
        assign(_InputIterator __first, _InputIterator __last)
        {
           typedef typename std::__is_integer<_InputIterator>::__type _Integral;
           _M_assign_dispatch(__first, __last, _Integral());
        }
#endif

#if __cplusplus >= 201103L
      //>参数为initializer_list
      //>直接调用迭代器版本的assign函数, 代码简洁之道
      void
      assign(initializer_list<value_type> __l)
      { this->assign(__l.begin(), __l.end()); }
#endif

      //>使用基类的分配器
      using _Base::get_allocator;

      //>下面是几个不同版本的begin/end函数
      //>用于获取不同版本的迭代器iterator/const_iterator
      //>begin返回的是指向vector第一个元素的迭代器
      //>end返回的是指向vector最后一个元素的下一个位置的迭代器
      iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_start); }

      const_iterator
      begin() const _GLIBCXX_NOEXCEPT
      { return const_iterator(this->_M_impl._M_start); }

      iterator
      end() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_finish); }

      const_iterator
      end() const _GLIBCXX_NOEXCEPT
      { return const_iterator(this->_M_impl._M_finish); }

      //>下面是几个不同版本的rbegin/rend函数
      //>rbegin/rend与begin/end不同, 函数实现调用了end/begin, 返回的是返向迭代器
      //>用于获取不同版本的迭代器reverse_iterator/const_reverse_iterator
      //>rbegin返回的是指向vector最后一个元素的迭代器
      //>rend返回的是指向vector第一个元素的前一个位置的迭代器
      reverse_iterator
      rbegin() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(end()); }

      const_reverse_iterator
      rbegin() const _GLIBCXX_NOEXCEPT
      { return const_reverse_iterator(end()); }

      reverse_iterator
      rend() _GLIBCXX_NOEXCEPT
      { return reverse_iterator(begin()); }

      const_reverse_iterator
      rend() const _GLIBCXX_NOEXCEPT
      { return const_reverse_iterator(begin()); }

#if __cplusplus >= 201103L
      //>下面是几个const版本的迭代器函数
      //>cbegin/cend/crbegin/crend
      const_iterator
      cbegin() const noexcept
      { return const_iterator(this->_M_impl._M_start); }

      const_iterator
      cend() const noexcept
      { return const_iterator(this->_M_impl._M_finish); }

      const_reverse_iterator
      crbegin() const noexcept
      { return const_reverse_iterator(end()); }

      const_reverse_iterator
      crend() const noexcept
      { return const_reverse_iterator(begin()); }
#endif

      //>获取vector实有的元素个数
      size_type
      size() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }

      //>获取vector可容纳元素个数的最大值
      //>但是这个最大值仅仅是个可能值, 有可能根本就达不到
      size_type
      max_size() const _GLIBCXX_NOEXCEPT
      { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }

#if __cplusplus >= 201103L
      //>下面是几个版本的resize, 重置vector大小
      //>vector可能被截短或者以一定的方式追加元素
      //>size变大时调用_M_default_append追加
      //>size变小时调用_M_erase_at_end移除多余的
      void
      resize(size_type __new_size)
      {
        if (__new_size > size())
          _M_default_append(__new_size - size());
        else if (__new_size < size())
          _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }

      //>size变大时, 调用insert在vector尾端插入
      void
      resize(size_type __new_size, const value_type& __x)
      {
        if (__new_size > size())
          insert(end(), __new_size - size(), __x);
        else if (__new_size < size())
          _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }
#else
      //>带默认value_type的resize
      void
      resize(size_type __new_size, value_type __x = value_type())
      {
        if (__new_size > size())
          insert(end(), __new_size - size(), __x);
        else if (__new_size < size())
          _M_erase_at_end(this->_M_impl._M_start + __new_size);
      }
#endif

#if __cplusplus >= 201103L
      //>调整vector容量
      //>当capacity()大于size()时, 这个函数就起作用了
      //>前者大小会减小为后者大小
      //>通过_M_shrink_to_fit来实现
      void
      shrink_to_fit()
      { _M_shrink_to_fit(); }
#endif

      //>获取vector的容量
      //>从函数实现可以看出capacity不小于size
      size_type
      capacity() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_end_of_storage
			 - this->_M_impl._M_start); }

      //>判断vector是否为空
      //>实现是通过迭代器判断完成的
      bool
      empty() const _GLIBCXX_NOEXCEPT
      { return begin() == end(); }

      //>重置vector的容量, 使其接近于size()
      //>当__n超过max_size()时会有异常std::length_error
      void
      reserve(size_type __n);

      //>元素访问操作符, const与非const两个版本
      //>需要注意的是, 函数实现没有检查vector索引的有效性
      //>vector索引有可能越界, 后果...断错误
      reference
      operator[](size_type __n)
      { return *(this->_M_impl._M_start + __n); }

      const_reference
      operator[](size_type __n) const
      { return *(this->_M_impl._M_start + __n); }

    protected:
      //>vector索引的有效性检查, 用于at()
      //>索引越界时会抛出std::out_of_range异常
      void
      _M_range_check(size_type __n) const
      {
        if (__n >= this->size())
          __throw_out_of_range(__N("vector::_M_range_check"));
      }

    public:
      //>元素访问, const与非const两个版本
      //>调用了_M_range_check()进行索引的有效性检查
      reference
      at(size_type __n)
      {
        _M_range_check(__n);
        return (*this)[__n]; 
      }

      const_reference
      at(size_type __n) const
      {
        _M_range_check(__n);
        return (*this)[__n];
      }

      //>front/back返回vector的第一个/最后一个元素
      //>函数实现对是begin()/end()返回的迭代器取引用
      //>如果vector为空的话, 函数返回的结果是不可预料的...
      reference
      front()
      { return *begin(); }

      const_reference
      front() const
      { return *begin(); }

      reference
      back()
      { return *(end() - 1); }
      
      const_reference
      back() const
      { return *(end() - 1); }

      //>获取vector第一个元素, 返回指针
      //>函数实现调用了front()
#if __cplusplus >= 201103L
      _Tp*
#else
      pointer
#endif
      data() _GLIBCXX_NOEXCEPT
      { return std::__addressof(front()); }

#if __cplusplus >= 201103L
      const _Tp*
#else
      const_pointer
#endif
      data() const _GLIBCXX_NOEXCEPT
      { return std::__addressof(front()); }

      //>追加一个元素到vector末尾
      //>从函数实现可以看出, 当size()小于capacity()时
      //>在vector末尾直接构建一个元素, 常量耗时
      //>否则会根据__cplusplus版本做不同的事
      void
      push_back(const value_type& __x)
      {
        if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
        {
          _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
          ++this->_M_impl._M_finish;
        }
        else
#if __cplusplus >= 201103L
          _M_emplace_back_aux(__x);
#else
          _M_insert_aux(end(), __x);
#endif
      }

#if __cplusplus >= 201103L
      //>rvalue版本
      //>通过emplace_back实现
      void
      push_back(value_type&& __x)
      { emplace_back(std::move(__x)); }

      //>emplace_back与push_back不同
      //>emplace_back是在vector末尾直接构建一个元素
      //>而push_back是复制、移动一个元素到vector末尾
      //>不定参数的模板函数
      template<typename... _Args>
        void
        emplace_back(_Args&&... __args);
#endif

      //>从vector末尾移除一个元素
      //>操作的自然是_M_finish指针
      void
      pop_back()
      {
        --this->_M_impl._M_finish;
        _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
      }

#if __cplusplus >= 201103L
      //>在__position前emplace一个元素
      //>原理同emplace_back
      template<typename... _Args>
        iterator
        emplace(iterator __position, _Args&&... __args);
#endif

      //>下面是几个版本的insert
      //>在指定的位置插入元素
      iterator
      insert(iterator __position, const value_type& __x);

#if __cplusplus >= 201103L
      //>通过emplace来insert rvalue版本的元素
      iterator
      insert(iterator __position, value_type&& __x)
      { return emplace(__position, std::move(__x)); }

      //>参数为initializer_list时直接调用迭代器版本的insert
      void
      insert(iterator __position, initializer_list<value_type> __l)
      { this->insert(__position, __l.begin(), __l.end()); }
#endif

      //>调用_M_fill_insert
      void
      insert(iterator __position, size_type __n, const value_type& __x)
      { _M_fill_insert(__position, __n, __x); }

      //>迭代器版本的insert
      //>调用_M_insert_dispatch
#if __cplusplus >= 201103L
      template<typename _InputIterator,
	       typename = std::_RequireInputIter<_InputIterator>>
        void
        insert(iterator __position, _InputIterator __first,
	       _InputIterator __last)
        { _M_insert_dispatch(__position, __first, __last, __false_type()); }
#else
      template<typename _InputIterator>
        void
        insert(iterator __position, _InputIterator __first,
	       _InputIterator __last)
        {
          typedef typename std::__is_integer<_InputIterator>::__type _Integral;
          _M_insert_dispatch(__position, __first, __last, _Integral());
        }
#endif

      //>erase移除指定位置的元素
      iterator
      erase(iterator __position);

      //>erase移除指定范围的元素
      iterator
      erase(iterator __first, iterator __last);

      //>交换两个vector中的内容
      //>其实交换的就是_Vector_base::_Vector_impl中的三个指针
      void
      swap(vector& __x)
#if __cplusplus >= 201103L
        noexcept(_Alloc_traits::_S_nothrow_swap())
#endif
      {
        this->_M_impl._M_swap_data(__x._M_impl);
        _Alloc_traits::_S_on_swap(_M_get_Tp_allocator(),
          __x._M_get_Tp_allocator());
      }

      //>清空vector
      //>调用_M_erase_at_end
      void
      clear() _GLIBCXX_NOEXCEPT
      { _M_erase_at_end(this->_M_impl._M_start); }

    protected:
      //>_M_allocate_and_copy
      //>从函数名可以看出
      //>先allocate内存, 再copy数据
      //>使用了异常处理
      //>有异常时需要释放刚才分配的内存并再次抛出异常
      //>__try/__catch/__throw_exception_again定义在exception_define.h中
      /**********
      #ifndef __EXCEPTIONS
      // If -fno-exceptions, transform error handling code to work without it.
      # define __try  if (true)
      # define __catch(X)  if (false)
      # define __throw_exception_again
      #else
      // Else proceed normally.
      # define __try  try
      # define __catch(X)  catch(X)
      # define __throw_exception_again  throw
      #endif
      **********/
      template<typename _ForwardIterator>
        pointer
        _M_allocate_and_copy(size_type __n,
          _ForwardIterator __first, _ForwardIterator __last)
        {
          pointer __result = this->_M_allocate(__n);
          __try
          {
            std::__uninitialized_copy_a(__first, __last, __result,
              _M_get_Tp_allocator());
            return __result;
          }
          __catch(...)
          {
            _M_deallocate(__result, __n);
            __throw_exception_again;
          }
        }


      //>_M_initialize_dispatch
      //>从函数名可以看出
      //>分配了指定字节的内存后需要进行初始化
      //>初始化调用_M_fill_initialize
      template<typename _Integer>
        void
        _M_initialize_dispatch(_Integer __n, _Integer __value, __true_type)
        {
          this->_M_impl._M_start = _M_allocate(static_cast<size_type>(__n));
          this->_M_impl._M_end_of_storage =
            this->_M_impl._M_start + static_cast<size_type>(__n);
          _M_fill_initialize(static_cast<size_type>(__n), __value);
        }

      //>_M_initialize_dispatch
      //>迭代器版本
      //>初始化调用_M_range_initialize
      template<typename _InputIterator>
        void
        _M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
          __false_type)
        {
          typedef typename std::iterator_traits<_InputIterator>::
            iterator_category _IterCategory;
          _M_range_initialize(__first, __last, _IterCategory());
        }

      //>根据迭代器指向的内容初始化当前vector
      //>函数的第三个参数是个哑元
      //>根据哑元类型的不同, 有两个版本的_M_range_initialize
      template<typename _InputIterator>
        void
        _M_range_initialize(_InputIterator __first,
            _InputIterator __last, std::input_iterator_tag)
        {
          for (; __first != __last; ++__first)
#if __cplusplus >= 201103L
          emplace_back(*__first);
#else
          push_back(*__first);
#endif
        }

      template<typename _ForwardIterator>
        void
        _M_range_initialize(_ForwardIterator __first,
            _ForwardIterator __last, std::forward_iterator_tag)
        {
          const size_type __n = std::distance(__first, __last);
          this->_M_impl._M_start = this->_M_allocate(__n);
          this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
          this->_M_impl._M_finish =
            std::__uninitialized_copy_a(__first, __last,
              this->_M_impl._M_start,
              _M_get_Tp_allocator());
        }

      //>_M_fill_initialize
      void
      _M_fill_initialize(size_type __n, const value_type& __value)
      {
        std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value,
          _M_get_Tp_allocator());
        this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
      }

#if __cplusplus >= 201103L
      //>_M_default_initialize
      void
      _M_default_initialize(size_type __n)
      {
        std::__uninitialized_default_n_a(this->_M_impl._M_start, __n,
           _M_get_Tp_allocator());
        this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
      }
#endif

      //>两个_M_assign_dispatch函数
      template<typename _Integer>
        void
        _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
        { _M_fill_assign(__n, __val); }

      template<typename _InputIterator>
        void
        _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
            __false_type)
        {
          typedef typename std::iterator_traits<_InputIterator>::
            iterator_category _IterCategory;
          _M_assign_aux(__first, __last, _IterCategory());
        }

      //>两个_M_assign_aux函数
      template<typename _InputIterator>
        void
        _M_assign_aux(_InputIterator __first, _InputIterator __last,
            std::input_iterator_tag);

      template<typename _ForwardIterator>
        void
        _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
            std::forward_iterator_tag);

      //>_M_fill_assign
      void
      _M_fill_assign(size_type __n, const value_type& __val);

      //>两个_M_insert_dispatch函数
      template<typename _Integer>
        void
        _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
            __true_type)
        { _M_fill_insert(__pos, __n, __val); }

      template<typename _InputIterator>
        void
        _M_insert_dispatch(iterator __pos, _InputIterator __first,
            _InputIterator __last, __false_type)
        {
          typedef typename std::iterator_traits<_InputIterator>::
            iterator_category _IterCategory;
          _M_range_insert(__pos, __first, __last, _IterCategory());
        }

      //>两个_M_range_insert函数
      template<typename _InputIterator>
        void
        _M_range_insert(iterator __pos, _InputIterator __first,
            _InputIterator __last, std::input_iterator_tag);

      template<typename _ForwardIterator>
        void
        _M_range_insert(iterator __pos, _ForwardIterator __first,
            _ForwardIterator __last, std::forward_iterator_tag);

      //>_M_fill_insert
      void
      _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);

#if __cplusplus >= 201103L
      //>用于resize(n)
      void
      _M_default_append(size_type __n);

      //>_M_shrink_to_fit
      bool
      _M_shrink_to_fit();
#endif

#if __cplusplus < 201103L
      //>用于insert(p, x)
      void
      _M_insert_aux(iterator __position, const value_type& __x);
#else
      template<typename... _Args>
        void
        _M_insert_aux(iterator __position, _Args&&... __args);

      //>_M_emplace_back_aux
      template<typename... _Args>
        void
        _M_emplace_back_aux(_Args&&... __args);
#endif

      //>vector大小检查
      size_type
      _M_check_len(size_type __n, const char* __s) const
      {
        if (max_size() - size() < __n)
          __throw_length_error(__N(__s));

        const size_type __len = size() + std::max(size(), __n);
        return (__len < size() || __len > max_size()) ? max_size() : __len;
      }

      //>用于erase(q1, q2), clear(), resize(), _M_fill_assign
      void
      _M_erase_at_end(pointer __pos)
      {
        std::_Destroy(__pos, this->_M_impl._M_finish, _M_get_Tp_allocator());
        this->_M_impl._M_finish = __pos;
      }

#if __cplusplus >= 201103L
    private:
      //>两个_M_move_assign
      void
      _M_move_assign(vector&& __x, std::true_type) noexcept
      {
        vector __tmp(get_allocator());
        this->_M_impl._M_swap_data(__tmp._M_impl);
        this->_M_impl._M_swap_data(__x._M_impl);
        if (_Alloc_traits::_S_propagate_on_move_assign())
          std::__alloc_on_move(_M_get_Tp_allocator(),
            __x._M_get_Tp_allocator());
      }

      void
      _M_move_assign(vector&& __x, std::false_type)
      {
        if (__x._M_get_Tp_allocator() == this->_M_get_Tp_allocator())
          _M_move_assign(std::move(__x), std::true_type());
        else
        {
          this->assign(std::__make_move_if_noexcept_iterator(__x.begin()),
              std::__make_move_if_noexcept_iterator(__x.end()));
          __x.clear();
        }
      }
#endif
    };


  //>下面是几个全局的比较操作符, 全部inline
  //>两个基本的操作符是==和<
  //>其它所有的操作符都基于==和<
  template<typename _Tp, typename _Alloc>
    inline bool
    operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    {
      return (__x.size() == __y.size()
        && std::equal(__x.begin(), __x.end(), __y.begin())); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    {
      return std::lexicographical_compare(__x.begin(), __x.end(),
         __y.begin(), __y.end()); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    { return !(__x == __y); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    { return __y < __x; }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    { return !(__y < __x); }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
    { return !(__x < __y); }

  template<typename _Tp, typename _Alloc>
    inline void
    swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
    { __x.swap(__y); }

_GLIBCXX_END_NAMESPACE_CONTAINER
} // namespace std

#endif /* _STL_VECTOR_H */
