#include "slaver.h"


/*
Slaver::Slaver(const Navi *navi, LinearGraph &path, QObject *parent) : QObject(parent)
{
    condStruct(lock, cond, update, stopWorkFlag);
    path = path;
    navi = navi;
    parseFlag=true;
    stopListeningFlag=false;
    stopWorkFlag=false;
}*/

void Slaver::startParsing(std::string &filePath) {
    //Lock the mutex prior
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    //Check if a parse thread is currently running
    if (worker.joinable()) {
        //We obviously want it to stop, so do a stopWorker() call to let it stop at the next moment
        stopWorker();
        //There is already a process running, so we cannot run another process until the first has comnpleted
        worker.join();
    }
    //Signal, that we want to parse, rather than search for a path
    parseFlag=true;
    condStruct.stopWorking=false;
    //Start the parsing process, by assigning the worker thread to the parse method
    worker = std::thread(&Navi::parsePbfFile, navi, filePath, &condStruct);
}

void Slaver::startPathfinding() {

}

void Slaver::restartListener() {
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    if(!listener.joinable()) {
        condStruct.stopListening=false;
        //If its not running, we will restart it
        listener = std::thread(&Slaver::listenAndPropagate, this);
    }
    //Else this method will do nothing at all
    return;
}

void Slaver::listenAndPropagate() {

    int oldProgress=condStruct.progress;
    while (true) {
        std::unique_lock<std::mutex> lock(condStruct.mtx);
        //Will immedietly lock the mutex upon creation

        //We'll wait for a cond update and progress change
        condStruct.cond.wait(
                    lock, [this, oldProgress]{return (condStruct.progress!=oldProgress);}
                    );
        //At this point, something has changed and we have aquired the lock
        //But the notify may only be issued to wake us up and show us we have to stop!
        logger->info("Listener: Received Update! Propagating ...");
        if(condStruct.stopListening) {
            //So, we have to stop!
            return;
        }
        //No it was an actual change! What did happen while we were sleeping?
        if (parseFlag && condStruct.progress < 100) {
            //Emit a parse Update signal
            emit (parseProgress(condStruct.progress));
        }
        else if (parseFlag && condStruct.progress==100) {
            //Parsing done, omit a parse done signal
            emit (parsingDone(true));
        }
        else if (parseFlag && condStruct.progress>100) {
            //This means, that the parsing failed! Emit a done(false) signal
            emit(parsingDone(false));
        }
        else if (!parseFlag && condStruct.progress < 100) {
            //Emit pathfinding update signal
            emit(pathfindingProgress(condStruct.progress));
        }
        else if (!parseFlag && condStruct.progress==100) {
            //Emit pathfinding done signal
            emit(pathfindingDone(true));
        }
        else if (!parseFlag && condStruct.progress>100) {
            //This means the pathfinding has failed! Emit a done(false) signal
            emit(pathfindingDone(false));
        }
        //Update the progress variable
        oldProgress=condStruct.progress;
        //Finally make sure to release the lock again, and return to sleep again until the next update
        logger->info("... signal emitted.");
    }
}

void Slaver::stopListener() {
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    condStruct.stopListening=true;
    //Make a change to the update variable, such that the listener will wake up and see the stop-Signal
    condStruct.progress++;
    //Then send the wakeup call to the listener
    condStruct.cond.notify_all();
    //Now wait until the listener joins
    listener.join();
    return;
}

void Slaver::stopWorker() {
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    //Set the stop flag
    condStruct.stopWorking=true;
    //Maybe he is sleeping?
    condStruct.cond.notify_all();
    //Then wait for it to end in a secure state
    worker.join();
    return;
}


Slaver::~Slaver() {
    //We want to make sure that all threads have stopped, so send stop signals to them!
    stopListener();
    stopWorker();
}
