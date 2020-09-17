#ifndef _LIBS_UTILS_RWLOCK_H
#define _LIBS_UTILS_RWLOCK_H

#include <stdint.h>
#include <sys/types.h>

#if !defined(_WIN32)
# include <pthread.h>
#endif

#include <utils/Errors.h>

// 定义了线程相关的数据类型和优先级
#include <utils/ThreadDefs.h>

namespace android {

#if !defined(_WIN32)

// RWLock读写锁的特点是：
// 当读写锁处于写模式时
// 其后的读锁/写锁都会阻塞
// 当读写锁处于读模式时
// 其后的读锁仍可以获得控制权
// 但写锁将被阻塞以及这个写锁之后的读锁也会被阻塞
class RWLock {
public:
    // 同样用法的enum
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    // 类似于Mutex的构造/析构函数
    RWLock();
    RWLock(const char* name);
    RWLock(int type, const char* name = NULL);
    ~RWLock();

    // 几个读锁/写锁及解锁函数
    status_t readLock();
    status_t tryReadLock();
    status_t writeLock();
    status_t tryWriteLock();
    void     unlock();

    // 类似于Mutex的自动读锁
    class AutoRLock {
    public:
        inline AutoRLock(RWLock& rwlock) : mLock(rwlock)  { mLock.readLock(); }
        inline ~AutoRLock() { mLock.unlock(); }
    private:
        RWLock& mLock;
    };

    // 类似于Mutex的自动写锁
    class AutoWLock {
    public:
        inline AutoWLock(RWLock& rwlock) : mLock(rwlock)  { mLock.writeLock(); }
        inline ~AutoWLock() { mLock.unlock(); }
    private:
        RWLock& mLock;
    };

private:
    // 类似于Mutex的用法
    // 禁止RWLock拷贝
            RWLock(const RWLock&);
    RWLock& operator=(const RWLock&);

    // 对pthread_rwlock_t进行了封装
    pthread_rwlock_t mRWLock;
};

// 构造函数调用pthread_rwlock_init对读写锁进行初始化
inline RWLock::RWLock() {
    pthread_rwlock_init(&mRWLock, NULL);
}

// 构造函数调用pthread_rwlock_init对读写锁进行初始化
// unused的name没有实际用处
// 猜想大概是个备用参数
inline RWLock::RWLock(__attribute__((unused)) const char* name) {
    pthread_rwlock_init(&mRWLock, NULL);
}

// 当type为SHARED时
// 通过对应的pthread_rwlockattr_t设置其属性为PTHREAD_PROCESS_SHARED
// 以支持跨进程的同步
inline RWLock::RWLock(int type, __attribute__((unused)) const char* name) {
    if (type == SHARED) {
        pthread_rwlockattr_t attr;
        pthread_rwlockattr_init(&attr);
        pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_rwlock_init(&mRWLock, &attr);
        pthread_rwlockattr_destroy(&attr);
    } else {
        pthread_rwlock_init(&mRWLock, NULL);
    }
}

// 析构函数调用pthread_rwlock_destroy销毁读写锁
inline RWLock::~RWLock() {
    pthread_rwlock_destroy(&mRWLock);
}

// 调用pthread_rwlock_rdlock以获取读锁
inline status_t RWLock::readLock() {
    return -pthread_rwlock_rdlock(&mRWLock);
}

// 调用pthread_rwlock_tryrdlock以获取读锁并不会阻塞
inline status_t RWLock::tryReadLock() {
    return -pthread_rwlock_tryrdlock(&mRWLock);
}

// 调用pthread_rwlock_wrlock以获取写锁
inline status_t RWLock::writeLock() {
    return -pthread_rwlock_wrlock(&mRWLock);
}

// 调用pthread_rwlock_trywrlock以获取写锁并不会阻塞
inline status_t RWLock::tryWriteLock() {
    return -pthread_rwlock_trywrlock(&mRWLock);
}

// 调用pthread_rwlock_unlock以解锁
inline void RWLock::unlock() {
    pthread_rwlock_unlock(&mRWLock);
}

#endif // !defined(_WIN32)

// ---------------------------------------------------------------------------
}; // namespace android
// ---------------------------------------------------------------------------

#endif // _LIBS_UTILS_RWLOCK_H
