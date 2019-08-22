#include "threadPoll.h"

#include <iostream>
#include <deque>
#include <memory>
#include <unistd.h>
extern"C"{
    #include <pthread.h>
    #include <semaphore.h>
}

using namespace std;

/**
 *  create threads;init semaphore and mutex;init tastQueue
 * 
**/
template<class T> 
ThreadPoll<T> ::ThreadPoll(int maxThreadCount,int maxTaskCount){
    threadCount = maxThreadCount;
    taskQueueCapacity = maxTaskCount;
    mutex = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&sem,0,0);
    taskQueue.resize(0);

    pTids = new pthread_t[threadCount];
    

    flag = true;

    for(int i = 0;i < threadCount;i++){
        int ret = pthread_create(&pTids[i],NULL,ThreadFun,this);
        if(ret == 0){
            pthread_detach(pTids[i]);
        }
    }
}

/**
 *  close threads 
**/
template<class T>
ThreadPoll<T>::~ThreadPoll(){
    flag = 0;
    sleep(1);
}

/**
 *  append task to taskQueue
 *  return 0(success) : -1(faild)
**/
template<class T>
int ThreadPoll<T>::append(T * task){
    int ret = 0;
    pthread_mutex_lock(&mutex);
    if(taskQueue.size() >= taskQueueCapacity){
        ret = -1;
    }else{
        taskQueue.push_back(task);
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&sem);
    return ret;
}

template<class T>
void * ThreadPoll<T>::ThreadFun(void * arg){
    ThreadPoll *pThreadPoll = (ThreadPoll *)arg;
    pThreadPoll->run();
}

/**
 *  get task and 
 * 
 * 
**/
template<class T>
void ThreadPoll<T>::run(){
    T *task;
    while(flag){
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        task = taskQueue.front();
        taskQueue.pop_front();
        pthread_mutex_unlock(&mutex);
        if(task != NULL){
            task->process();
        }
    }
}

