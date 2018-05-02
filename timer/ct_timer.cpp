#include "ct_timer.hpp"
#include <chrono>

namespace ct{

CT_Timer::CT_Timer():
delay_(0),firstRun(true),times_(0),interval_(0),started(false)
{
}

CT_Timer::~CT_Timer(){
}

int64_t CT_Timer::Schedule(Handler handler,void *args,uint32_t delay,uint32_t interval,uint32_t times)
{
    cout<<"t id:"<<t.get_id()<<endl;
    if(started == false){
        handler_ = handler;
        delay_ = delay;
        times_ = times;
        interval_ = interval;
        thread t1(&CT_Timer::LoopTimer,this);
        t = std::move(t1);
        args_ = args;
        started = true;
        return (int64_t)(void *)&t;   
    }
    cout<<"schedule can be called only once!!"<<endl;
    return -1;
}

int64_t CT_Timer::Schedule(Handler handler,void *args)
{
    if(started == false)
    {
        handler_ = handler;
        args_ = args;
        t = thread(&CT_Timer::LoopThread,this);
        started = true;
        return (int64_t)(void *)&t;   
    }
    return -1;
}

bool CT_Timer::SendCmd(long cmd)
{
    q.enqueue(cmd);
}

void CT_Timer::Join(){
   if(started)
    {
        t.join();
    }
}

int32_t CT_Timer::Cancel()
{
    if(started == false)
    {
        cout<<"timer not started"<<endl;
        return -1;
    }
    q.enqueue(CMD_EXIT);
//    t.detach();
    return 0;
}

void CT_Timer::LoopTimer(){
    uint32_t tmptimes = 0;
    uint32_t handlerRuntime = 0;

    cout<<"t id:"<<t.get_id()<<endl;
    while(true)
    {

        long item = -1; 
        if(firstRun && delay_ > 0)
        {   
            q.wait_dequeue_timed(item,chrono::milliseconds(delay_));
        //    firstRun = false;
            if(item == CMD_EXIT)
                break;
        }
        if(firstRun)
        {
            auto begin = chrono::steady_clock::now();
            handler_(args_,item);
            auto end = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-begin);
            handlerRuntime = duration.count();
            cout<<"handler cost "<<handlerRuntime<<endl;
            tmptimes++;
            firstRun = false;
        }
        if(tmptimes < times_ || times_ == -1) 
        {
            uint32_t tmpInterval = (interval_ - handlerRuntime);
            q.wait_dequeue_timed(item,chrono::milliseconds(tmpInterval>0?tmpInterval:0)); 
            if(item == CMD_EXIT)
            {
                cout<<"rcv CMD_EXIT"<<endl;
                break;
            }
            auto begin = chrono::steady_clock::now();
            handler_(args_,item);
            auto end = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-begin);
            handlerRuntime = duration.count();
            tmptimes++;
        }
    }
    started = false;
}

void CT_Timer::LoopThread()
{
    while(true)
    {
        int item = -1;
        q.wait_dequeue(item);
        if(item == CMD_EXIT)
        {
            cout<<"rcv CMD_EXIT"<<endl;
            break;
        }
        cout<<"item="<<item<<endl;
        handler_(args_,item);
    }
}

}
