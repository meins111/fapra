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
    QWidget *ecar;
    QCheckBox *isECar;
    QLineEdit *ecarReach;
    QLabel *ecarReachLabel;
    QComboBox *reachUnit;
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


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 2);

        tabWidget->addTab(route, QString());
        ecar = new QWidget();
        ecar->setObjectName(QStringLiteral("ecar"));
        isECar = new QCheckBox(ecar);
        isECar->setObjectName(QStringLiteral("isECar"));
        isECar->setGeometry(QRect(10, 10, 121, 20));
        ecarReach = new QLineEdit(ecar);
        ecarReach->setObjectName(QStringLiteral("ecarReach"));
        ecarReach->setGeometry(QRect(80, 50, 271, 22));
        ecarReachLabel = new QLabel(ecar);
        ecarReachLabel->setObjectName(QStringLiteral("ecarReachLabel"));
        ecarReachLabel->setGeometry(QRect(10, 50, 71, 16));
        reachUnit = new QComboBox(ecar);
        reachUnit->setObjectName(QStringLiteral("reachUnit"));
        reachUnit->setGeometry(QRect(360, 50, 72, 22));
        tabWidget->addTab(ecar, QString());
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

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        movementType->clear();
        movementType->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Auto", 0)
         << QApplication::translate("Widget", "Fahrrad", 0)
         << QApplication::translate("Widget", "Fu\303\237g\303\244nger", 0)
        );
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
        tabWidget->setTabText(tabWidget->indexOf(route), QApplication::translate("Widget", "Route", 0));
        isECar->setText(QApplication::translate("Widget", "Route f\303\274r E-Car", 0));
        ecarReachLabel->setText(QApplication::translate("Widget", "Reichweite", 0));
        reachUnit->clear();
        reachUnit->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Km", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(ecar), QApplication::translate("Widget", "E-Car", 0));
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
