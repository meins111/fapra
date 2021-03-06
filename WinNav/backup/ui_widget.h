/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
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
    QLineEdit *startLatitude;
    QLineEdit *endLatitude;
    QLabel *longitudeLabel;
    QRadioButton *endRadio;
    QLineEdit *startLongitude;
    QPushButton *calculateButton;
    QLabel *latitudeLabel;
    QRadioButton *startRadio;
    QLineEdit *endLongitude;
    QRadioButton *noneRadio;
    QComboBox *movementType;
    QComboBox *optimizeBy;
    QWidget *ecar;
    QCheckBox *isECar;
    QLineEdit *ecarReach;
    QLabel *ecarReachLabel;
    QComboBox *reachUnit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(480, 350);
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
        startLatitude = new QLineEdit(route);
        startLatitude->setObjectName(QStringLiteral("startLatitude"));

        gridLayout->addWidget(startLatitude, 1, 2, 1, 1);

        endLatitude = new QLineEdit(route);
        endLatitude->setObjectName(QStringLiteral("endLatitude"));

        gridLayout->addWidget(endLatitude, 2, 2, 1, 1);

        longitudeLabel = new QLabel(route);
        longitudeLabel->setObjectName(QStringLiteral("longitudeLabel"));

        gridLayout->addWidget(longitudeLabel, 0, 1, 1, 1);

        endRadio = new QRadioButton(route);
        endRadio->setObjectName(QStringLiteral("endRadio"));

        gridLayout->addWidget(endRadio, 2, 0, 1, 1);

        startLongitude = new QLineEdit(route);
        startLongitude->setObjectName(QStringLiteral("startLongitude"));

        gridLayout->addWidget(startLongitude, 1, 1, 1, 1);

        calculateButton = new QPushButton(route);
        calculateButton->setObjectName(QStringLiteral("calculateButton"));

        gridLayout->addWidget(calculateButton, 1, 3, 1, 1);

        latitudeLabel = new QLabel(route);
        latitudeLabel->setObjectName(QStringLiteral("latitudeLabel"));

        gridLayout->addWidget(latitudeLabel, 0, 2, 1, 1);

        startRadio = new QRadioButton(route);
        startRadio->setObjectName(QStringLiteral("startRadio"));

        gridLayout->addWidget(startRadio, 1, 0, 1, 1);

        endLongitude = new QLineEdit(route);
        endLongitude->setObjectName(QStringLiteral("endLongitude"));

        gridLayout->addWidget(endLongitude, 2, 1, 1, 1);

        noneRadio = new QRadioButton(route);
        noneRadio->setObjectName(QStringLiteral("noneRadio"));
        noneRadio->setChecked(true);

        gridLayout->addWidget(noneRadio, 3, 0, 1, 1);

        movementType = new QComboBox(route);
        movementType->setObjectName(QStringLiteral("movementType"));

        gridLayout->addWidget(movementType, 2, 3, 1, 1);

        optimizeBy = new QComboBox(route);
        optimizeBy->setObjectName(QStringLiteral("optimizeBy"));

        gridLayout->addWidget(optimizeBy, 3, 3, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

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

        gridLayout_2->addWidget(tabWidget, 0, 1, 1, 1);


        verticalLayout->addWidget(topWidget);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        longitudeLabel->setText(QApplication::translate("Widget", "Longitude", 0));
        endRadio->setText(QApplication::translate("Widget", "End", 0));
        calculateButton->setText(QApplication::translate("Widget", "Route berechnen", 0));
        latitudeLabel->setText(QApplication::translate("Widget", "Latitude", 0));
        startRadio->setText(QApplication::translate("Widget", "Start", 0));
        noneRadio->setText(QApplication::translate("Widget", "None", 0));
        movementType->clear();
        movementType->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Auto", 0)
         << QApplication::translate("Widget", "Fahrrad", 0)
        );
        optimizeBy->clear();
        optimizeBy->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Schnellste", 0)
         << QApplication::translate("Widget", "K\303\274rzeste", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(route), QApplication::translate("Widget", "Route", 0));
        isECar->setText(QApplication::translate("Widget", "Route f\303\274r E-Car", 0));
        ecarReachLabel->setText(QApplication::translate("Widget", "Reichweite", 0));
        reachUnit->clear();
        reachUnit->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Km", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(ecar), QApplication::translate("Widget", "E-Car", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
