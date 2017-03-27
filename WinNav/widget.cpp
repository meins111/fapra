#include "widget.h"
#include "ui_widget.h"
#include "logwriter.h"
#include <sstream>
#include <cmath>
#include <pthread.h>
#include <cstdlib>


using namespace std;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    logger(el::Loggers::getLogger("default")),
    slave(&navi, path, this),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->topWidget->setFixedHeight(200);
    //Progress bar and labels setup
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->routingProgressBar->setMinimum(0);
    ui->routingProgressBar->setMaximum(100);
    ui->sucessFailureLabel->setText("");
    ui->messageLabel->setText("");
    //Parking tab setup
    ui->parkingAllowanceComboBox->hide();
    ui->parkingResultLabel->hide();
    ui->parkingTypeLabel->hide();
    ui->freeParkingBox->hide();
    ui->timeConstraintCheckbox->hide();
    graphToggle=false;

	 mapWidget = new MapWidget(this);

    mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
	 mapWidget->setCenterLatitude(48.83);
	 mapWidget->setCenterLongitude(9.18);
	 mapWidget->setZoom(2400);
     //Connect Slot&Signals
	 QObject::connect( mapWidget, &MapWidget::mouseClickGeoPosition, 
			 this, &Widget::mouseClickGeoPosition );
     QObject::connect( ui->calculateButton, &::QPushButton::clicked, this, &Widget::startRouting );
     QObject::connect(ui->loadFile, &::QPushButton::clicked, this, &Widget::loadPushed);
     QObject::connect(ui->parkingEnableBox, &::QCheckBox::stateChanged, this, &Widget::parkingToggle);
     QObject::connect(&slave, SIGNAL (parseProgress(int)), this, SLOT(parseProgress(int)));
     QObject::connect(&slave, SIGNAL(parsingDone(int)), this, SLOT(parsingDone(int)));
     QObject::connect(&slave, SIGNAL(pathfindingProgress(int)), this, SLOT(pathfindingProgress(int)));
     QObject::connect(&slave, SIGNAL(pathfindingDone(int)), this, SLOT(pathfindingDone(int)));
     QVBoxLayout *layout = dynamic_cast<QVBoxLayout *> (this->layout());
	 layout->QLayout::addWidget(mapWidget);

	 string path = this->getPbfPath();

	 logger->info("Try to load the default osm file: %v ...", path);
     ui->messageLabel->setText("Parsing...");
	 slave.startParsing(path);
}

Widget::~Widget()
{
	delete mapWidget;
    delete ui;
}

void Widget::mouseClickGeoPosition(qreal lon, qreal lat, Marble::GeoDataCoordinates::Unit unit ) {
	if(unit == Marble::GeoDataCoordinates::Radian) {
		lon = lon * 180 / M_PI;
		lat = lat * 180 / M_PI;
	}

	QString lonString = QString::number(lon);
	QString latString = QString::number(lat);

	if( ui->startRadio->isChecked() ) {

		ui->startLongitude->setText( lonString );
		ui->startLatitude->setText( latString );
		mapWidget->getGraphLayer().setStart(lon, lat);

	} else if( ui->endRadio->isChecked() ) {

		ui->endLongitude->setText( lonString );
		ui->endLatitude->setText( latString );
		mapWidget->getGraphLayer().setEnd(lon, lat);

	}
}

void Widget::startRouting() {
    //First check if the navGraph is parsed already
    if (!navi.isNavGraphParsed()) {
        ui->sucessFailureLabel->setText("Aborted: Parsing incomplete!");
        return;
    }
    //Reset the route
    path.reset();
    //Reset gui progress indicators
    ui->routingProgressBar->setValue(0);
    ui->sucessFailureLabel->setText("Routing...");
//void Widget::on_pushButton_clicked(){
	switch( ui->movementType->currentIndex() ) {
		default:
		case MOVEMENT_CAR:
            navi.setTravelMedium(CAR);
			break;
        case MOVEMENT_BIKE:
            navi.setTravelMedium(BIKE);
            break;
		case MOVEMENT_FOOT:
            navi.setTravelMedium(FOOT);
			break;
	}

	switch( ui->optimizeBy->currentIndex() ) {
		default:
		case OPTIMIZE_FASTEST:
            navi.setRoutingPriority(true);
			break;
		case OPTIMIZE_SHORTEST:
            navi.setRoutingPriority(false);
			break;
	}
    //Fetch parking informations (if any)
    if (ui->parkingEnableBox->isChecked()) {
        navi.enableParkingSearch();
        bool publicParking=false, privateParking=false, customerParking=false;
        switch (ui->parkingAllowanceComboBox->currentIndex()) {
        case PUBLIC:
            publicParking=true;
            break;
        case CUSTOMER:
            customerParking=true;
            break;
        case PRIVATE:
            privateParking=true;
            break;
        default:
            //keep at all at false!
            break;
        }
        bool timeConstraint = false, freeParking = true;
        if (ui->timeConstraintCheckbox->isChecked()) {
            timeConstraint=true;
        }
        if (ui->freeParkingBox->isChecked()) {
            freeParking=false;
        }
        //Set the parking search parameters
        navi.setParkingParameters(publicParking, privateParking, customerParking, freeParking, timeConstraint);
    }
    else {
        navi.disableParkingSearch();
    }


    ///Test-Graph
    //mapWidget->getGraphLayer().setGraph(testGraph);
    //mapWidget->update();

    //Fetch start/end coords
    //Set english decimal dot as forced locale for deciaml dot
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");
    double startLon, startLat, endLat, endLon;
    std::string tmp = ui->startLongitude->text().toStdString();
    startLon = std::stod(tmp, NULL);
    startLat = atof(ui->startLatitude->text().toStdString().c_str());
    endLon = atof(ui->endLongitude->text().toStdString().c_str());
    endLat = atof(ui->endLatitude->text().toStdString().c_str());
    PODNode start(startLon, startLat);
    PODNode target(endLon, endLat);
    slave.setStart(start);
    slave.setTarget(target);
    //Let the Slaver start the pathfinding
    logger->info("Start Pathfinding from (%v :: %v) to (%v :: %v)!", startLon, startLat, endLon, endLat);
    slave.startPathfinding();
}


void Widget::loadPushed() {
    std::string path (ui->filePath->text().toStdString());
    //string path = this->getPbfPath();

    logger->info("Try to load specified osm file: %v ...", path);
    ui->messageLabel->setText("Parsing...");
    ui->progressBar->setValue(0);
    slave.startParsing(path);


    //slaves.startNavGraphParsing(path, &navi);
    //navi.buildNavGraph(path);
    //ui->progressBar->setValue(ui->progressBar->maximum());
}


void Widget::parseProgress(int percentProgress) {
    if (percentProgress>100 || percentProgress<0) {
        //Redirect to error handling
        parsingDone(percentProgress);
        return;
    }
    //Else we just update the progess
    ui->progressBar->setValue(percentProgress);
    return;
}

void Widget::parsingDone(int returnCode) {
    //Check for errors
    if (returnCode>100 || returnCode<0) {
        stringstream errString;
        errString << "FAILED with Error-Code: " << returnCode;
        string errMsg = errString.str();
        ui->sucessFailureLabel->setText(QString::fromStdString(errMsg));
        return;
    }
    //Else set the progress to 100 and set success label message
    ui->progressBar->setValue(100);
    ui->messageLabel->setText("Done!");

    return;
}

void Widget::pathfindingProgress(int percentProgress){
    //TODO: update the progress bar if any
    if (percentProgress>100 || percentProgress<0) {
        //Redirect to error handling
        pathfindingDone(percentProgress);
        return;
    }
    //Else we just update the progess
    ui->routingProgressBar->setValue(percentProgress);
    return;
}

void Widget::pathfindingDone(int returnCode) {
    //Update the gui progress elements according to outcome of the operation
    //Check for errors
    if (returnCode>100 || returnCode<0) {
        ui->routingProgressBar->setValue(0);
        stringstream errString;
        errString << "FAILED with Error-Code: " << returnCode;
        string errMsg = errString.str();
        ui->sucessFailureLabel->setText(QString::fromStdString(errMsg));
        //May Print all expanded nodes for debugging -> enable methods in AStar and Navi class
        /*std::vector<PODNode> visited;
        navi.getVisitedNodes(visited);
        path.insertNodes(visited);
        mapWidget->getGraphLayer().setGraph(path);
        mapWidget->update();
        */
        return;
    }
    //Else set the progress to 100 and set success label message
    else {
        ui->routingProgressBar->setValue(100);
        ui->sucessFailureLabel->setText("SUCCESS!");
    }
    navi.getShortestRouteGraph(path);
    //Check the returned graph
    if (path.numNodes()<2) {
        //This seems to be broken!
        ui->sucessFailureLabel->setText("Error: Path empty!");
        return;
    }
    //Get shortest path stats
    double routeTime=navi.getShortestRouteTime()/60;
    stringstream streamTime;
    streamTime << std::fixed << std::setprecision(0) << routeTime;
    string stringTime = streamTime.str();

    double routeDist=navi.getShortestRouteDistance()/1000;
    stringstream streamDist;
    streamDist << fixed << setprecision(1) << routeDist;
    string stringDist = streamDist.str();

    std::string labelString = "Dist: " + stringDist + " km\nTime: " + stringTime + " min";
    ui->routeDetailsLabel->setText(QString::fromStdString(labelString));
    /*
    if (navi.getRoutingPrio()) {
        //Quickest path, so the cost is given in travel time, unit is seconds
        routeCost /= 60;    //In minutes
        long intCost = std::round(routeCost);
        std::string cost = "Travel Time: " + std::to_string(intCost) + " min.";
        ui->routeDetailsLabel->setText(QString::fromStdString(cost));
    }
    else {
        //Shortest Path in terms of travel distance, unit is meters
        routeCost /= 1000;    //In kilometers
        long intCost = std::round(routeCost);
        std::string cost = "Travel Distance: " + std::to_string(intCost) + " km.";
        ui->routeDetailsLabel->setText(QString::fromStdString(cost));
    }
    */
    mapWidget->getGraphLayer().setGraph(path);
    mapWidget->update();
    mapWidget->setCenterLatitude(path.getNode(0).getLatitude());
    mapWidget->setCenterLongitude(path.getNode(0).getLongitude());
    //TODO: Get and draw the path if successful
    /*
    //Draw it
    mapWidget->getGraphLayer().setGraph(path);
    mapWidget->update();
    //Zoom to the start node
    mapWidget->setCenterLatitude(path.getNode(0).getLatitude());
    mapWidget->setCenterLongitude(path.getNode(0).getLongitude());
    */
}

std::string Widget::getPbfPath() {
	std::string path;
	if( !getenv("PBFPATH") ) {

		path = "/home/jochen/Downloads/stuttgart-regbez-latest.osm.pbf";
        return path;
    }

	path = getenv("HOME");
	path += getenv("PBFPATH");

	return path;
}


void Widget::on_showGraph_clicked()
{
    if (!graphToggle) {
        graphToggle=true;
        ui->showGraph->setText("Hide Nav Graph");
        navi.getFullGraph(path);
        mapWidget->getGraphLayer().setGraph(path);
        mapWidget->update();
        return;
    }
    else {
        graphToggle=false;
        ui->showGraph->setText("Show Nav Graph");
        path.reset();
        mapWidget->getGraphLayer().setGraph(path);
        mapWidget->update();
        return;
    }
}

void::Widget::parkingToggle() {
    //check whether it is checked or not
    if (ui->parkingEnableBox->isChecked()) {
        ui->parkingAllowanceComboBox->show();
        ui->parkingResultLabel->show();
        ui->parkingResultLabel->setText("");
        ui->parkingTypeLabel->show();
        ui->freeParkingBox->show();
        ui->timeConstraintCheckbox->show();
    }
    else {
        //Not enabled, hide additionl parking options
        ui->parkingAllowanceComboBox->hide();
        ui->parkingResultLabel->hide();
        ui->parkingTypeLabel->hide();
        ui->freeParkingBox->hide();
        ui->timeConstraintCheckbox->hide();
    }
}

