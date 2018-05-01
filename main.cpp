#include "ct_timer.hpp"

int32_t  handler(void *args,long cmd)
{
    cout<<"in handler1"<<endl;
    return 0;
}

class Test{
public:
    Test(){
    }
    ~Test(){
    }
    int32_t operator()(void *args,long cmd){
        cout<<"in handler2"<<endl;
        return 0;
    }
};

auto hlr = [=](void *args,long cmd)->int32_t{
   cout<<"in handler 3"<<endl;
};


int main(int argc,char *argv[]){
    ct::CT_Timer timer;
    Test test;
    int64_t t = timer.Schedule(test,nullptr,5000,2000,10);
    //timer.Schedule(hlr,nullptr,5000,2000,10);
    this_thread::sleep_for(chrono::seconds(20));
    timer.Cancel();
    timer.join();
    return 0;
}
