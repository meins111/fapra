#include "widget.h"
#include <QApplication>
#include <QVBoxLayout>
#include "Utils/easylogging++.h"

#include <vector>
#include "Utils/inplacevectorreorder.h"

INITIALIZE_EASYLOGGINGPP
int main(int argc, char *argv[])
{
/*
    std::vector<int> A = {8, 6, 7, 5, 3, 0, 9};
    std::vector<size_t> newIndex = {3, 6, 2, 4, 0, 1, 5};
    inplaceReorder(A, newIndex);
*/
    LOG(INFO) << "Programm started.";
    QApplication a(argc, argv);

    Widget w;
    w.show();


    return a.exec();
}
