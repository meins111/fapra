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
    //Check if a parse thread is currently running
    if (worker.joinable()) {
        //We obviously want it to stop, so do a stopWorker() call to let it stop at the next moment
        stopWorker();
    }
    //Signal, that we want to parse, rather than search for a path
    parseFlag=true;
    condStruct.stopWorking=false;
    //Start the parsing process, by assigning the worker thread to the parse method
    worker = std::thread(&Navi::parsePbfFile, navi, filePath, &condStruct);
}

void Slaver::startPathfinding() {
    //Check if start/target nodes are set
    if (!startNodeSet) {
        //Emit an error
        emit (pathfindingDone(STARTNODE_NOT_SET));
        return;
    }
    if (!targetNodeSet) {
        //emit an error
        emit (pathfindingDone(TARGETNODE_NOT_SET));
        return;
    }
    //Make sure no worker is currently running (signal him to stop if one is running)
    stopWorker();
    //Stop and restart the listener such that he listens for the upcoming pathfinding
    stopListener();
    //Lock the struct (just to be sure, in fact it should not be a problem at all, since both threads are stopped by now)
    std::unique_lock<std::mutex>uLock(condStruct.mtx);
    //We are pathfinding now, not parsing
    parseFlag=false;
    //Reset condStruct values
    condStruct.progress=0;
    condStruct.stopListening=false;
    condStruct.stopWorking=false;
    //All changes are set, free the lock
    uLock.unlock();
    //And restart the listener
    restartListener();

    //Now start the pathfinding-worker thread
    worker=std::thread(&Navi::shortestPath, navi, startNode, targetNode, &condStruct);

}


void Slaver::startMetaGraphBuilder() {
    //if the building is already running, ignore the new call!
    if (metaBuilder.joinable() || metaListener.joinable()) {
        return;
    }
    //Don't start the builder before the initial parsing is done
    if (!navi->isNavGraphParsed()) {
        return;
    }
    //Start up the listener
    metaListener = std::thread(&Slaver::metaBuildListener, this);
    //Then start the builder
    metaBuilder = std::thread(&Navi::buildMetaGraph, navi, &condMetaStruct);
}

void Slaver::metaBuildListener() {
    bool notDone = true;
    int oldProgress=condMetaStruct.progress;
    while (notDone) {
        std::unique_lock<std::mutex> lock(condMetaStruct.mtx);
        //Will immedietly lock the mutex upon creation
        //We'll wait for a cond update and progress change
        condMetaStruct.cond.wait(
                    lock, [this, oldProgress]{return (condMetaStruct.progress!=oldProgress);}
                    );
        //At this point, something has changed and we have aquired the lock
        //But the notify may only be issued to wake us up and show us we have to stop!
        if(condMetaStruct.stopListening) {
            //So, we have to stop!
            return;
        }
        //No it was an actual change! What did happen while we were sleeping?
        if (condMetaStruct.progress < 100 && condMetaStruct.progress >= 0) {
            //An progress! emit a signal to notify the gui of this
            emit(metaGraphProgress(condMetaStruct.progress));
            continue;
        }
        else if (condMetaStruct.progress<0 || condMetaStruct.progress > 100) {
            //This is an error!
            emit(metaGraphDone(condMetaStruct.progress));
            //Meta graph builder has reported an error, so shut down the listener as well
            return;
        }
        else {
            //We are done!
            emit(metaGraphDone(condMetaStruct.progress));
            //Meta graph is build correctly, so shut down the listener as well
            return;
        }
    }
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
            emit (parsingDone(100));
        }
        else if (parseFlag && condStruct.progress>100) {
            //This means, that the parsing failed! Emit a done(false) signal
            emit(parsingDone(condStruct.progress));
        }
        else if (!parseFlag && condStruct.progress < 100) {
            //Emit pathfinding update signal
            emit(pathfindingProgress(condStruct.progress));
        }
        else if (!parseFlag && condStruct.progress==100) {
            //Emit pathfinding done signal
            emit(pathfindingDone(100));
        }
        else if (!parseFlag && condStruct.progress>100) {
            //This means the pathfinding has failed! Emit a done(false) signal
            emit(pathfindingDone(condStruct.progress));
        }
        //Update the progress variable
        oldProgress=condStruct.progress;
    }
}

void Slaver::stopListener() {
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    //Is the listener running at all?
    if(!listener.joinable()) {
        //No listener currently running, so just exit!
        return;
    }
    condStruct.stopListening=true;
    //Make a change to the update variable, such that the listener will wake up and see the stop-Signal
    condStruct.progress++;
    //Then send the wakeup call to the listener
    condStruct.cond.notify_all();
    //Free the lock and wait for the listener to shut down
    uLock.unlock();
    //Now wait until the listener joins
    listener.join();
    return;
}

void Slaver::stopWorker() {
    std::unique_lock<std::mutex> uLock(condStruct.mtx);
    //Is a worker running at all?
    if (!worker.joinable()) {
        //No worker running, so just exit!
        return;
    }
    //Else we have to signal the worker to stop: Set the stop flag
    condStruct.stopWorking=true;
    //Maybe he is sleeping? Notify him!
    condStruct.cond.notify_all();
    //Free the lock and wait for the worker to shut down
    uLock.unlock();
    //Then wait for it to end in a secure state
    worker.join();
    return;
}

void Slaver::setStart(PODNode start) {
    startNode=start;
    startNodeSet=true;
}

void Slaver::setTarget(PODNode target) {
    targetNode=target;
    targetNodeSet=true;
}

/*
void Slaver::setTravelMedium(TravelMedium medium) {
    navi->setTravelMedium(medium);
}

void Slaver::setRoutingPriority (bool timeIsPrio) {
    navi->setRoutingPriority(timeIsPrio);
}



void Slaver::setRange(double maxRange) {
    navi->setMaxRange(maxRange);
}
*/

void Slaver::stopMetaThreads() {
    std::unique_lock<std::mutex> uLock(condMetaStruct.mtx);
    condMetaStruct.stopListening=true;
    condMetaStruct.stopWorking=true;
    condMetaStruct.cond.notify_all();
    uLock.unlock();
    if (metaListener.joinable()) {
        metaListener.join();
    }
    if (metaBuilder.joinable()) {
        metaBuilder.join();
    }
}

Slaver::~Slaver() {
    //We want to make sure that all threads have stopped, so send stop signals to them!
    stopListener();
    stopWorker();
    stopMetaThreads();
}
