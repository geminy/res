// 宏定义惯例
#ifndef _LIBS_UTILS_MUTEX_H
#define _LIBS_UTILS_MUTEX_H

// C99引入的标准C库的头文件
// 定义了几种扩展的整数类型和宏
#include <stdint.h>

// 基本系统数据类型
#include <sys/types.h>

// 日期和时间相关函数
#include <time.h>

// 平台相关
// pthread即POSIX线程
// 定义了一系列线程及同步相关的接口
#if !defined(_WIN32)
# include <pthread.h>
#endif

// Android自定义了errno
#include <utils/Errors.h>

// Android自定义了时间格式转换函数
#include <utils/Timers.h>

// android名字空间
namespace android {

// Condition类前置声明
// 作为Mutex类的友元
class Condition;

class Mutex {
public:
    // PRIVATE限于同一进程内的多线程同步
    // SHARED支持进程间同步
    // 从下面的Mutex构造函数可以看出两者的区别
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

	// 几个构造/析构函数
    Mutex();
    Mutex(const char* name);
    Mutex(int type, const char* name = NULL);
    ~Mutex();

	// 类似于pthread的lock/unlock/trylock
    status_t lock();
    void     unlock();
    status_t tryLock();

    // 平台相关
    // timedLock获取锁时限制了等待时间
#if defined(__ANDROID__)
    status_t timedLock(nsecs_t timeoutMilliseconds);
#endif

    // 自动加锁解锁
    class Autolock {
    public:
        // 构造对象时加锁
        inline Autolock(Mutex& mutex) : mLock(mutex)  { mLock.lock(); }
        inline Autolock(Mutex* mutex) : mLock(*mutex) { mLock.lock(); }
        // 销毁对象时解锁
        inline ~Autolock() { mLock.unlock(); }
    private:
        // 对象类型为引用时只能通过初始化列表进行初始化
        Mutex& mLock;
    };

private:
    // 友元声明
    friend class Condition;

    // 拷贝构造函数和赋值操作符私有声明而不定义
    // Mutex不能被拷贝
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);

    // 平台相关
	// 非_WIN32时对pthread的mutex进行封装
#if !defined(_WIN32)
    pthread_mutex_t mMutex;
#else
    void  _init();
    void* mState;
#endif
};

// ---------------------------------------------------------------------------

#if !defined(_WIN32)

// 构造函数调用pthread_mutex_init初始化mutex
inline Mutex::Mutex() {
    pthread_mutex_init(&mMutex, NULL);
}

// 构造函数调用pthread_mutex_init初始化mutex
// __attribute__是GCC的一个特性
// unused表示其后的实参name没有被使用
inline Mutex::Mutex(__attribute__((unused)) const char* name) {
    pthread_mutex_init(&mMutex, NULL);
}

// 指定mutex类型的构造函数
// 当type为SHARED时
// 通过pthread_mutexattr_t设置其属性为PTHREAD_PROCESS_SHARED
// 表示mutex支持多进程
inline Mutex::Mutex(int type, __attribute__((unused)) const char* name) {
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        pthread_mutex_init(&mMutex, NULL);
    }
}

// 析构函数调用pthread_mutex_destroy销毁mutex对象
inline Mutex::~Mutex() {
    pthread_mutex_destroy(&mMutex);
}

// lock加锁时调用了pthread_mutex_lock
// 锁被占用时会阻塞
// 成功时返回0
// 失败时返回一个errno
// 这些errno和status_t类型在utils/Errors.h中定义
inline status_t Mutex::lock() {
    return -pthread_mutex_lock(&mMutex);
}

// unlock解锁时调用了pthread_mutex_unlock
inline void Mutex::unlock() {
    pthread_mutex_unlock(&mMutex);
}

// trylock加锁时调用了pthread_mutex_trylock
// 锁被占用时会直接返回二不会等待
inline status_t Mutex::tryLock() {
    return -pthread_mutex_trylock(&mMutex);
}

// 特有的timedLock
// 加锁阻塞时限制了等待的时间
// nsecs_t在utils/Timers.h中定义
// timespec结构体的两个成员是秒和纳秒
#if defined(__ANDROID__)
inline status_t Mutex::timedLock(nsecs_t timeoutNs) {
    const struct timespec ts = {
        /* .tv_sec = */ static_cast<time_t>(timeoutNs / 1000000000),
        /* .tv_nsec = */ static_cast<long>(timeoutNs % 1000000000),
    };
    return -pthread_mutex_timedlock(&mMutex, &ts);
}
#endif

#endif // !defined(_WIN32)

// ---------------------------------------------------------------------------

// AutoMutex惯用法：
// 在函数中需要加锁时先声明一个AutoMutex局部变量
// AutoMutex局部变量自动销毁时便会解锁
typedef Mutex::Autolock AutoMutex;

}; // namespace android

#endif // _LIBS_UTILS_MUTEX_H
