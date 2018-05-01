#include "ct_timer.hpp"

int32_t  handler(void *args)
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
    int32_t operator()(void *args){
        cout<<"in handler2"<<endl;
        return 0;
    }
};

auto hlr = [=](void *args)->int32_t{
   cout<<"in handler 3"<<endl;
};


int main(int argc,char *argv[]){
    ct::CT_Timer timer;
    Test test;
    //timer.Schedule(test,nullptr,5000,2000,10);
    timer.Schedule(hlr,nullptr,5000,2000,10);
    this_thread::sleep_for(chrono::seconds(20));
    timer.Cancel();
    this_thread::sleep_for(chrono::seconds(100));
    return 0;
}
