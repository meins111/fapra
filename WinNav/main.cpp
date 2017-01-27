#include "widget.h"
#include <QApplication>
#include <QVBoxLayout>
#include "Utils/easylogging++.h"

INITIALIZE_EASYLOGGINGPP
int main(int argc, char *argv[])
{
    LOG(INFO) << "Programm started.";
    QApplication a(argc, argv);

    Widget w;
    w.show();


    return a.exec();
}
