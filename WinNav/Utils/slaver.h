#ifndef SLAVER_H
#define SLAVER_H

#include <QObject>
#include "TestGraph/lineargraph.h"
#include "Pathfinding/navi.h"
#include <thread>
#include "Utils/condwait_t.h"
#include "logwriter.h"
#include "Utils/ErrorCodes.h"


class Slaver : public QObject
{
    Q_OBJECT
public:
    ~Slaver();
    Slaver(Navi *navi, LinearGraph &path, QObject *parent) : QObject(parent), navi(navi), path(path), parseFlag(true),
        listener(&Slaver::listenAndPropagate, this), logger(el::Loggers::getLogger("default")),
            startNodeSet(false), targetNodeSet(false){}
    void startParsing(std::string &filePath);
    void startPathfinding();
    void startMetaGraphBuilder();
    void stopListener();
    void stopMetaThreads();
    void restartListener();
    void stopWorker();

    void setStart(PODNode start);
    void setTarget(PODNode target);
    /*
    //Setter for pathfinding settings
    void setTravelMedium(TravelMedium medium);
    void setRoutingPriority (bool timeIsPrio);
    void setRange(double maxRange);
    */

protected:
    Navi *navi;
    LinearGraph path;

    CondWait_t condStruct;
    CondWait_t condMetaStruct;
    bool parseFlag;

    std::thread worker;
    std::thread listener;
    std::thread metaListener;
    std::thread metaBuilder;

    el::Logger* logger;

    void listenAndPropagate();
    void metaBuildListener();

    //Local copy of the start/target nodes
    PODNode startNode, targetNode;
    bool startNodeSet, targetNodeSet;



signals:
    void parseProgress(int percentProgress);
    void parsingDone(int successFlag);

    void pathfindingProgress(int percentProgress);
    void pathfindingDone(int successFlag);

    void metaGraphProgress(int percentProgress);
    void metaGraphDone(int successFlag);

};

#endif // SLAVER_H
