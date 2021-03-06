#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <marble/GeoDataCoordinates.h>
#include "MapWidget/MapWidget.h"
#include "Pathfinding/navi.h"
#include "Pathfinding/nodeinfo.h"
#include "Pathfinding/basicedge.h"
#include "Pathfinding/navgraph.h"
#include "TestGraph/PODNode.h"
#include "TestGraph/lineargraph.h"
#include "Utils/slaver.h"
#include "Utils/easylogging++.h"
#include <stdio.h>
#include <iomanip> // setprecision
#include <sstream> // stringstream


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    //TestGraph testGraph;
	 // Anpassen nach den Ids in der qt-generator `movementType` combobox
     enum MOVEMENT_TYPE { MOVEMENT_CAR = 0x4, MOVEMENT_BIKE = 0x2, MOVEMENT_FOOT = 0x1 };

	 // Anpassen nach den Ids in der qt-generator `movementType` combobox
     enum OPTIMIZE_BY { OPTIMIZE_FASTEST = 0, OPTIMIZE_SHORTEST = 1 };

	 // Anpassen nach den Ids in der qt-generator `reachUnit` combobox
	 enum REACH_UNIT { REACH_KM = 0 };

     enum PARK_ALLOWANCE { PUBLIC=0, CUSTOMER=1, PRIVATE=2 };


public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    MapWidget *mapWidget;

public slots:
	void mouseClickGeoPosition (qreal lon, qreal lat, Marble::GeoDataCoordinates::Unit);
    void startRouting();
    void loadPushed();
    void parkingToggle();
    //void on_pushButton_clicked();

    void parseProgress(int percentProgress);
    void parsingDone(int returnCode);

    void pathfindingProgress(int percentProgress);
    void pathfindingDone(int returnCode);

private slots:
    void on_showGraph_clicked();
    float toFloat(std::string str);

private:
    Ui::Widget *ui;
    //Initialize with car movement and time priority
    Navi navi;
    LinearGraph path;
    Slaver slave;
    el::Logger* logger;
    bool graphToggle;

	 std::string getPbfPath();
};

#endif // WIDGET_H
