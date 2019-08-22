#include <iostream>
#include <unistd.h>
#include "threadPoll.h"
#include "threadPoll.cpp"

class my_task{
    public:
    my_task(){};
    void process(){
        cout << "test" << endl;
    }
};

int main()
{
    ThreadPoll<my_task> *threadPoll = new ThreadPoll<my_task>(4,10);
    
    my_task tasks[5];
    for(int i = 0;i < 5;i++){
        threadPoll->append(&tasks[i]);
        sleep(1);
    }

}