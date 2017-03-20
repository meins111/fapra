/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *topWidget;
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *route;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QLineEdit *endLongitude;
    QComboBox *movementType;
    QLineEdit *startLatitude;
    QLineEdit *startLongitude;
    QLineEdit *endLatitude;
    QRadioButton *startRadio;
    QComboBox *optimizeBy;
    QRadioButton *noneRadio;
    QRadioButton *endRadio;
    QLabel *sucessFailureLabel;
    QLabel *longitudeLabel;
    QLabel *latitudeLabel;
    QPushButton *calculateButton;
    QProgressBar *routingProgressBar;
    QLabel *routeDetailsLabel;
    QWidget *parking;
    QCheckBox *parkingEnableBox;
    QCheckBox *freeParkingBox;
    QComboBox *parkingAllowanceComboBox;
    QLabel *parkingTypeLabel;
    QLabel *parkingResultLabel;
    QWidget *Settings;
    QLineEdit *filePath;
    QPushButton *loadFile;
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QLabel *messageLabel;
    QPushButton *showGraph;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(504, 350);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        topWidget = new QWidget(Widget);
        topWidget->setObjectName(QStringLiteral("topWidget"));
        gridLayout_2 = new QGridLayout(topWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tabWidget = new QTabWidget(topWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        route = new QWidget();
        route->setObjectName(QStringLiteral("route"));
        gridLayout_3 = new QGridLayout(route);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        endLongitude = new QLineEdit(route);
        endLongitude->setObjectName(QStringLiteral("endLongitude"));

        gridLayout->addWidget(endLongitude, 10, 1, 1, 1);

        movementType = new QComboBox(route);
        movementType->setObjectName(QStringLiteral("movementType"));

        gridLayout->addWidget(movementType, 10, 3, 1, 1);

        startLatitude = new QLineEdit(route);
        startLatitude->setObjectName(QStringLiteral("startLatitude"));

        gridLayout->addWidget(startLatitude, 5, 2, 1, 1);

        startLongitude = new QLineEdit(route);
        startLongitude->setObjectName(QStringLiteral("startLongitude"));

        gridLayout->addWidget(startLongitude, 5, 1, 1, 1);

        endLatitude = new QLineEdit(route);
        endLatitude->setObjectName(QStringLiteral("endLatitude"));

        gridLayout->addWidget(endLatitude, 10, 2, 1, 1);

        startRadio = new QRadioButton(route);
        startRadio->setObjectName(QStringLiteral("startRadio"));

        gridLayout->addWidget(startRadio, 5, 0, 1, 1);

        optimizeBy = new QComboBox(route);
        optimizeBy->setObjectName(QStringLiteral("optimizeBy"));

        gridLayout->addWidget(optimizeBy, 11, 3, 1, 1);

        noneRadio = new QRadioButton(route);
        noneRadio->setObjectName(QStringLiteral("noneRadio"));
        noneRadio->setChecked(true);

        gridLayout->addWidget(noneRadio, 11, 0, 1, 1);

        endRadio = new QRadioButton(route);
        endRadio->setObjectName(QStringLiteral("endRadio"));

        gridLayout->addWidget(endRadio, 10, 0, 1, 1);

        sucessFailureLabel = new QLabel(route);
        sucessFailureLabel->setObjectName(QStringLiteral("sucessFailureLabel"));

        gridLayout->addWidget(sucessFailureLabel, 11, 4, 1, 1);

        longitudeLabel = new QLabel(route);
        longitudeLabel->setObjectName(QStringLiteral("longitudeLabel"));

        gridLayout->addWidget(longitudeLabel, 4, 1, 1, 1);

        latitudeLabel = new QLabel(route);
        latitudeLabel->setObjectName(QStringLiteral("latitudeLabel"));

        gridLayout->addWidget(latitudeLabel, 4, 2, 1, 1);

        calculateButton = new QPushButton(route);
        calculateButton->setObjectName(QStringLiteral("calculateButton"));

        gridLayout->addWidget(calculateButton, 5, 4, 1, 1);

        routingProgressBar = new QProgressBar(route);
        routingProgressBar->setObjectName(QStringLiteral("routingProgressBar"));
        routingProgressBar->setValue(0);

        gridLayout->addWidget(routingProgressBar, 10, 4, 1, 1);

        routeDetailsLabel = new QLabel(route);
        routeDetailsLabel->setObjectName(QStringLiteral("routeDetailsLabel"));

        gridLayout->addWidget(routeDetailsLabel, 4, 4, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 2);

        tabWidget->addTab(route, QString());
        parking = new QWidget();
        parking->setObjectName(QStringLiteral("parking"));
        parkingEnableBox = new QCheckBox(parking);
        parkingEnableBox->setObjectName(QStringLiteral("parkingEnableBox"));
        parkingEnableBox->setGeometry(QRect(10, 30, 92, 23));
        freeParkingBox = new QCheckBox(parking);
        freeParkingBox->setObjectName(QStringLiteral("freeParkingBox"));
        freeParkingBox->setGeometry(QRect(30, 60, 121, 23));
        parkingAllowanceComboBox = new QComboBox(parking);
        parkingAllowanceComboBox->setObjectName(QStringLiteral("parkingAllowanceComboBox"));
        parkingAllowanceComboBox->setGeometry(QRect(160, 90, 86, 25));
        parkingTypeLabel = new QLabel(parking);
        parkingTypeLabel->setObjectName(QStringLiteral("parkingTypeLabel"));
        parkingTypeLabel->setGeometry(QRect(50, 90, 81, 17));
        parkingTypeLabel->setTextFormat(Qt::AutoText);
        parkingResultLabel = new QLabel(parking);
        parkingResultLabel->setObjectName(QStringLiteral("parkingResultLabel"));
        parkingResultLabel->setGeometry(QRect(260, 10, 191, 111));
        tabWidget->addTab(parking, QString());
        Settings = new QWidget();
        Settings->setObjectName(QStringLiteral("Settings"));
        filePath = new QLineEdit(Settings);
        filePath->setObjectName(QStringLiteral("filePath"));
        filePath->setGeometry(QRect(20, 30, 321, 27));
        loadFile = new QPushButton(Settings);
        loadFile->setObjectName(QStringLiteral("loadFile"));
        loadFile->setGeometry(QRect(358, 30, 81, 27));
        statusLabel = new QLabel(Settings);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setGeometry(QRect(20, 90, 51, 20));
        progressBar = new QProgressBar(Settings);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(100, 90, 118, 23));
        progressBar->setMaximum(8);
        progressBar->setValue(0);
        messageLabel = new QLabel(Settings);
        messageLabel->setObjectName(QStringLiteral("messageLabel"));
        messageLabel->setGeometry(QRect(120, 120, 67, 17));
        showGraph = new QPushButton(Settings);
        showGraph->setObjectName(QStringLiteral("showGraph"));
        showGraph->setGeometry(QRect(308, 80, 131, 25));
        tabWidget->addTab(Settings, QString());

        gridLayout_2->addWidget(tabWidget, 0, 1, 1, 1);


        verticalLayout->addWidget(topWidget);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        endLongitude->setText(QApplication::translate("Widget", "9.42389", 0));
        movementType->clear();
        movementType->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Auto", 0)
         << QApplication::translate("Widget", "Fahrrad", 0)
         << QApplication::translate("Widget", "Fu\303\237g\303\244nger", 0)
        );
        startLatitude->setText(QApplication::translate("Widget", "48.9422", 0));
        startLongitude->setText(QApplication::translate("Widget", "9.30089", 0));
        endLatitude->setText(QApplication::translate("Widget", "48.691", 0));
        startRadio->setText(QApplication::translate("Widget", "Start", 0));
        optimizeBy->clear();
        optimizeBy->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Schnellste", 0)
         << QApplication::translate("Widget", "K\303\274rzeste", 0)
        );
        noneRadio->setText(QApplication::translate("Widget", "None", 0));
        endRadio->setText(QApplication::translate("Widget", "End", 0));
        sucessFailureLabel->setText(QString());
        longitudeLabel->setText(QApplication::translate("Widget", "Longitude", 0));
        latitudeLabel->setText(QApplication::translate("Widget", "Latitude", 0));
        calculateButton->setText(QApplication::translate("Widget", "Route berechnen", 0));
        routeDetailsLabel->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(route), QApplication::translate("Widget", "Route", 0));
        parkingEnableBox->setText(QApplication::translate("Widget", "Parking", 0));
        freeParkingBox->setText(QApplication::translate("Widget", "Free Parking", 0));
        parkingAllowanceComboBox->clear();
        parkingAllowanceComboBox->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Public", 0)
         << QApplication::translate("Widget", "Time-Constraint", 0)
         << QApplication::translate("Widget", "Private", 0)
         << QApplication::translate("Widget", "Customer", 0)
        );
        parkingTypeLabel->setText(QApplication::translate("Widget", "Allowance", 0));
        parkingResultLabel->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(parking), QApplication::translate("Widget", "Parking", 0));
        filePath->setText(QApplication::translate("Widget", ".../Path to OSM.pbf File", 0));
        loadFile->setText(QApplication::translate("Widget", "Parse File", 0));
        statusLabel->setText(QApplication::translate("Widget", "Status:", 0));
        messageLabel->setText(QString());
        showGraph->setText(QApplication::translate("Widget", "Show Nav Graph", 0));
        tabWidget->setTabText(tabWidget->indexOf(Settings), QApplication::translate("Widget", "Parsing", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
