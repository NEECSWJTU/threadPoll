#ifndef _THREADPOLL
#define _THREADPOLL

#include <iostream>
#include <deque>
#include <memory>

extern"C"{
    #include <pthread.h>
    #include <semaphore.h>
}
using namespace std;

/**
 * members:
 *  deque<T *> workQueue;
 *  pthread_mutex mutex;
 *  semaphore sem;
 *  
 *  pthread_t *tids;
 *  bool flags *tids;
 * 
 *  int threadCount;
 * 
 * functions:
 *  ThreadPoll(int threadCount);
 *  ~ThradPoll();
 * 
 *  static void threadFun();
 *  void run();
 *  int append(T *t);
 * 
**/
template<class T>
class ThreadPoll{
private:
    deque<T *> taskQueue;
    pthread_mutex_t mutex;
    sem_t sem;

    shared_ptr<pthread_t> tids;
    pthread_t *pTids;
    bool flag;

    int threadCount;
    int taskQueueCapacity;

    void run();

public:
    ThreadPoll(int maxThreadCount,int maxTaskCount);
    ~ThreadPoll();

    static void * ThreadFun(void * arg);

    int append(T* task);
};
#endif