#ifndef SLAVER_H
#define SLAVER_H

#include <QObject>
#include "TestGraph/lineargraph.h"
#include "Pathfinding/navi.h"
#include <thread>
#include "Utils/condwait_t.h"
#include "logwriter.h"


class Slaver : public QObject
{
    Q_OBJECT
public:
    ~Slaver();
    Slaver(Navi *navi, LinearGraph &path, QObject *parent) : QObject(parent), logger(el::Loggers::getLogger("default")), path(path),
        navi(navi), parseFlag(true), listener(&Slaver::listenAndPropagate, this){}
    void startParsing(std::string &filePath);
    void startPathfinding();    //NYI!!
    void stopListener();
    void restartListener();
    void stopWorker();

protected:
    Navi *navi;
    LinearGraph path;

    CondWait_t condStruct;
    bool parseFlag;

    std::thread worker;
    std::thread listener;

    el::Logger* logger;

    void listenAndPropagate();

signals:
    void parseProgress(int percentProgress);
    void parsingDone(bool successFlag);

    void pathfindingProgress(int percentProgress);
    void pathfindingDone(bool successFlag);

};

#endif // SLAVER_H
