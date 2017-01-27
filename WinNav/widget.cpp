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
     QObject::connect(&slave, SIGNAL (parseProgress(int)), this, SLOT(parseProgress(int)));
     QObject::connect(&slave, SIGNAL(parsingDone(bool)), this, SLOT(parsingDone(bool)));
     QObject::connect(&slave, SIGNAL(pathfindingProgress(int)), this, SLOT(pathfindingProgress(int)));
     QObject::connect(&slave, SIGNAL(pathfindingDone(bool)), this, SLOT(pathfindingDone(bool)));
     QVBoxLayout *layout = dynamic_cast<QVBoxLayout *> (this->layout());
	 layout->QLayout::addWidget(mapWidget);

	 string path = this->getPbfPath();

	  
	 logger->info("Try to load the default osm file: %v ...", path);
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
    //Reset the route
    path.reset();
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
            navi.routingPriority(true);
			break;
		case OPTIMIZE_SHORTEST:
            navi.routingPriority(false);
			break;
	}

	// navigator.setECar ( ui->isECar->isChecked() );
	unsigned reachInM;
	unsigned reachInKm = atoi( ui->ecarReach->text().toStdString().c_str() );

	switch( ui->reachUnit->currentIndex() ) {
		default:
		case REACH_KM:
			reachInM = reachInKm * 1000;
			break;
	}
	// navigator.setECarReach(reachInM);

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
    try {
        printf("Start A* ...\n");
        //route=navi.shortestPath(start, target);
        printf(" ... A* finished!\n");

        //Draw it
        mapWidget->getGraphLayer().setGraph(path);
        mapWidget->update();
        //Zoom to the start node
        mapWidget->setCenterLatitude(path.getNode(0).getLatitude());
        mapWidget->setCenterLongitude(path.getNode(0).getLongitude());
    }
    catch (...) {
        //Could not find a valid path or parsing failed / was not done
        printf("A*-Exception");
    }
}


void Widget::loadPushed() {
    std::string path (ui->filePath->text().toStdString());
    //slaves.startNavGraphParsing(path, &navi);
    //navi.buildNavGraph(path);
    //ui->progressBar->setValue(ui->progressBar->maximum());
}


void Widget::parseProgress(int percentProgress) {
    //TODO: Update the progrss bar if any
}

void Widget::parsingDone(bool successFlag) {
    //Todo: toggle some indicator to show that processing has completed
}

void Widget::pathfindingProgress(int percentProgress){
    //TODO: update the progress bar if any
}

void Widget::pathfindingDone(bool successFlag) {
    //TODO: Draw the path
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

