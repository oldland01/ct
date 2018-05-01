#ifndef CT_TIMER_HPP
#define CT_TIMER_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include "concurrentqueue.h"
#include "blockingconcurrentqueue.h"

using namespace std;

namespace ct{
typedef std::function<int32_t (void *args,long cmd)> Handler;
enum cmd_t{
    CMD_EXIT = 0
};
class CT_Timer{
public:
    CT_Timer();
    virtual ~CT_Timer();
    int64_t Schedule(Handler handler,void *args,uint32_t delay,uint32_t interval,uint32_t times);
    bool SendCmd(long cmd);
    int32_t Cancel();
    void join();
private:
    void loop();
    moodycamel::BlockingConcurrentQueue<long> q;
    thread t;
    uint32_t delay_;
    bool firstRun;
    uint32_t times_;
    uint32_t interval_;
    void *args_;
    Handler handler_;
    atomic<bool> started;
};

} //ct

#endif
