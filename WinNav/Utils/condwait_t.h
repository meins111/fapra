#ifndef CONDWAIT_T
#define CONDWAIT_T

#include <thread>
#include <mutex>
#include <condition_variable>
#include "stopworkingexception.h"

typedef struct CondWait{
    std::condition_variable cond;
    int progress;
    bool stopWorking;
    bool stopListening;
    std::mutex mtx;
    CondWait (): stopWorking(false), stopListening(false), progress(0) {}

    void updateProgress(int currentProgress) {
        if ( stopWorking )
            throw StopWorkingException();
        std::unique_lock<std::mutex> lock( mtx );
        this->progress = currentProgress;
        cond.notify_all();
    }

//private:

} CondWait_t;

#endif // CONDWAIT_T

