#ifndef ANDROID_BARRIER_H
#define ANDROID_BARRIER_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/threads.h>

namespace android {

// Barrier集合了Mutex和Condition
// 所谓的条件变量通过enum值OPENED和CLOSED实现
// 提供了3个函数open/close/wait
// 这3个函数都对state进行了读/写
// 所以需要加锁保护
// 否则会造成死锁等意想不到的现象
class Barrier
{
public:
    // 初始状态为CLOSED
    inline Barrier() : state(CLOSED) { }
    inline ~Barrier() { }

    // open函数设置state为OPENED
    // 调用broadcast函数解除wait函数的阻塞状态
    void open() {
        Mutex::Autolock _l(lock);
        state = OPENED;
        cv.broadcast();
    }

    // close函数重置state为CLOSED
    // 这时将阻塞wait函数
    void close() {
        Mutex::Autolock _l(lock);
        state = CLOSED;
    }

    // 当state为OPEND时
    // wait被唤醒
    // 从而退出wait
    void wait() const {
        Mutex::Autolock _l(lock);
        while (state == CLOSED) {
            cv.wait(lock);
        }
    }
private:
    enum { OPENED, CLOSED };
    mutable  Mutex     lock;
    mutable  Condition cv;
    volatile int       state;
};

}; // namespace android

#endif // ANDROID_BARRIER_H
