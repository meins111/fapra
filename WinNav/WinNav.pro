#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T14:27:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32 {
    INCLUDEPATH += $$quote(C:/Program Files/marble/include/marble)
    LIBS += $$quote(C:/Program Files/marble/libmarblewidget.dll)
    LIBS += $$quote(C:\Program Files (x86)\marble\libastro.dll)
}
unix {
    LIBS += -L/usr/lib/x86_64-linux-gnu \
    -lmarblewidget-qt5
    LIBS += -pthread
}

TARGET = ParkMe-Navigator
TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS += -pthread


SOURCES +=        main.cpp\
                  widget.cpp\
                  GraphInterface/Node.cpp\
                  GraphInterface/Edge.cpp\
                  GraphInterface/Graph.cpp\
		  GraphLayer/GraphLayer.cpp\
		  TestGraph/PODEdge.cpp\
                  TestGraph/PODNode.cpp\
		  MapWidget/MapWidget.cpp \
    #moc_widget.cpp \
    Pathfinding/basicgraph.cpp \
    Pathfinding/basicnode.cpp \
    Pathfinding/basicedge.cpp \
    Pathfinding/nodeinfo.cpp \
    Pathfinding/edgeinfo.cpp \
    Pathfinding/navgraph.cpp \
    Pathfinding/navi.cpp \
    Pathfinding/pathfinder.cpp \
    Utils/inplacevectorreorder.cpp \
    TestGraph/lineargraph.cpp \
    Utils/slaver.cpp\
    #moc_slaver.cpp
    Utils/easylogging++.cc \
    GraphInterface/pointofinterest.cpp \
    stopworkingexception.cpp \
    Pathfinding/astar.cpp \
    Pathfinding/ParkingSolution.cpp


HEADERS  += widget.h\
            GraphLayer/GraphLayer.h\
            TestGraph/PODEdge.h\
            TestGraph/PODNode.h\
            MapWidget/MapWidget.h \
            backup/ui_widget.h \
            GraphInterface/Edge.h \
            GraphInterface/Graph.h \
            GraphInterface/Node.h \
            ui_widget.h \
    Pathfinding/basicgraph.h \
    Pathfinding/basicnode.h \
    Pathfinding/basicedge.h \
    Pathfinding/nodeinfo.h \
    Pathfinding/edgeinfo.h \
    Pathfinding/allowancemasks.h \
    Pathfinding/roadtypes.h \
    Pathfinding/closenesstree.h \
    Pathfinding/navgraph.h \
    Pathfinding/navi.h \
    Pathfinding/pathfinder.h \
    Utils/inplacevectorreorder.h \
    TestGraph/lineargraph.h \
    Utils/slaver.h \
    Utils/condwait_t.h \
    Utils/easylogging++.h \
    GraphInterface/pointofinterest.h \
    stopworkingexception.h \
    Pathfinding/astar.h \
    Utils/ErrorCodes.h \
    Pathfinding/ParkingSolution.h \
    Utils/OsmFilterContainer.h

FORMS    += widget.ui


#osmpbf parse library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../osmpbf-master/osmpbf/release/ -losmpbf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../osmpbf-master/osmpbf/debug/ -losmpbf
else:unix: LIBS += -L$$PWD/../osmpbf/osmpbf/ -losmpbf


INCLUDEPATH += $$PWD/../osmpbf
INCLUDEPATH += $$PWD/../osmpbf/osmpbf/include
DEPENDPATH += $$PWD/../osmpbf
DEPENDPATH += $$PWD/../osmpbf/osmpbf/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../osmpbf/osmpbf/release/libosmpbf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../osmpbf/osmpbf/debug/libosmpbf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../osmpbf/osmpbf/release/osmpbf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../osmpbf/osmpbf/debug/osmpbf.lib
else:unix: PRE_TARGETDEPS += $$PWD/../osmpbf/osmpbf/libosmpbf.a


#google's protobuf lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../../usr/include/google/protobuf
DEPENDPATH += $$PWD/../../../../../../usr/include/google/protobuf

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/libprotobuf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/protobuf.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/libprotobuf.a

#qt-marble widget lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/release/ -lmarblewidget-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/debug/ -lmarblewidget-qt5
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lmarblewidget-qt5

INCLUDEPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu
INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += += $$PWD/../../../../../../usr/include


#nanoflann library - header-only library, so we need only the include path to use it
INCLUDEPATH += $$PWD/../nanoflann/include
DEPENDPATH += $$PWD/../nanoflann/include

#boost library include path
INCLUDEPATH += /usr/local/boost_1_63_0/





#zlib include
unix:!macx: LIBS += -lz

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/release/ -lz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/debug/ -lz
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lz

INCLUDEPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/release/libz.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/debug/libz.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/release/z.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/debug/z.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/x86_64-linux-gnu/libz.a
