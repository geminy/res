#ifndef _LIBS_UTILS_CONDITION_H
#define _LIBS_UTILS_CONDITION_H

#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#if !defined(_WIN32)
# include <pthread.h>
#endif

#include <utils/Errors.h>
#include <utils/Mutex.h>
#include <utils/Timers.h>

namespace android {

// 条件变量实际上是对互斥锁的一种扩展
// 条件变量允许线程阻塞并等待其它的线程发信号将其唤醒
class Condition {
public:
	// PRIVATE和SHARED的含义同Mutex中的用法
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    // 唤醒等待者
    // 唤醒一个还是全部
    // 对应于signal和broadcast
    enum WakeUpType {
        WAKE_UP_ONE = 0,
        WAKE_UP_ALL = 1
    };

    Condition();
    Condition(int type);
    ~Condition();

    // 阻塞式等待条件变量或者设定延时等待时间
    status_t wait(Mutex& mutex);
    status_t waitRelative(Mutex& mutex, nsecs_t reltime);

    // 唤醒单个等待者或者所有等待者
    void signal();
    void signal(WakeUpType type) {
        if (type == WAKE_UP_ONE) {
            signal();
        } else {
            broadcast();
        }
    }
    void broadcast();

private:
// 封装了pthread_cond_t条件变量
#if !defined(_WIN32)
    pthread_cond_t mCond;
#else
    void* mState;
#endif
};

// ---------------------------------------------------------------------------

#if !defined(_WIN32)

// 构造函数调用pthread_cond_init初始化条件变量
inline Condition::Condition() {
    pthread_cond_init(&mCond, NULL);
}

// 原理同Mutex
// 当类型为SHARED时
// 通过pthread_condattr_t设置其属性为PTHREAD_PROCESS_SHARED
// 以支持进程间的同步
inline Condition::Condition(int type) {
    if (type == SHARED) {
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&mCond, &attr);
        pthread_condattr_destroy(&attr);
    } else {
        pthread_cond_init(&mCond, NULL);
    }
}

// 析构函数调用pthread_cond_destroy销毁条件变量
inline Condition::~Condition() {
    pthread_cond_destroy(&mCond);
}

// 调用pthread_cond_wait等待条件变量
// 这个wait机制比较有意思
// wait前首先要给mutex加锁
// wait内部会先给mutex解锁然后等待
// 直到条件满足时再给mutex加锁并返回
// 这些操作是一个原子操作
inline status_t Condition::wait(Mutex& mutex) {
    return -pthread_cond_wait(&mCond, &mutex.mMutex);
}

// timedWait设定了延时等待时间
inline status_t Condition::waitRelative(Mutex& mutex, nsecs_t reltime) {
#if defined(HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE)
    struct timespec ts;
    ts.tv_sec  = reltime / 1000000000;
    ts.tv_nsec = reltime % 1000000000;
    return -pthread_cond_timedwait_relative_np(&mCond, &mutex.mMutex, &ts);
#else // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
    struct timespec ts;
#if defined(__linux__)
    clock_gettime(CLOCK_REALTIME, &ts);
#else // __APPLE__
    struct timeval t;
    gettimeofday(&t, NULL);
    ts.tv_sec = t.tv_sec;
    ts.tv_nsec= t.tv_usec * 1000;
#endif
    ts.tv_sec  += reltime / 1000000000;
    ts.tv_nsec += reltime % 1000000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec  += 1;
    }
    return -pthread_cond_timedwait(&mCond, &mutex.mMutex, &ts);
#endif // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
}

// 调用pthread_cond_signal唤醒某个等待者
inline void Condition::signal() {
    pthread_cond_signal(&mCond);
}

// 调用pthread_cond_broadcast唤醒所有等待者
inline void Condition::broadcast() {
    pthread_cond_broadcast(&mCond);
}

#endif // !defined(_WIN32)

}; // namespace android

#endif // _LIBS_UTILS_CONDITON_H
