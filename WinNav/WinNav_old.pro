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
}

TARGET = WinNav
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        widget.cpp\
		  Graph/Node.cpp\
		  Graph/Edge.cpp\
		  Graph/Graph.cpp\
                    Graph/Route.cpp\
		  GraphLayer/GraphLayer.cpp\
		  TestGraph/PODEdge.cpp\
		  TestGraph/PODNode.cpp\
		  TestGraph/TestGraph.cpp\
                    Pathfinding/NavGraph.cpp\
                    Pathfinding/Navigator.cpp\
		  MapWidget/MapWidget.cpp


HEADERS  += widget.h\
				Graph/Node.h\
				Graph/Edge.h\
				Graph/Graph.h\
                                Graph/Route.h\
		  		GraphLayer/GraphLayer.h\
				TestGraph/PODEdge.h\
				TestGraph/PODNode.h\
				TestGraph/TestGraph.h\
                                Pathfinding/NavGraph.h\
                                Pathfinding/Navigator.h\
				MapWidget/MapWidget.h

FORMS    += widget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/release/ -losmpbf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/debug/ -losmpbf
else:unix: LIBS += /home/bjn/Uni/osm_fapra/osmpbf/osmpbf/ -losmpbf

#INCLUDEPATH += $$PWD/../osmpbf/osmpbf/include
INCLUDEPATH += /home/bjn/Uni/osm_fapra/osmpbf/osmpbf/include
#INCLUDEPATH += $$PWD/../../osm_project/dependencies/nanoflann/nanoflann/include
INCLUDEPATH += /home/bjn/Uni/osm_fapra/nanoflann/include

INCLUDEPATH += /home/bjn/Uni/osm_fapra/osmpbf/
INCLUDEPATH += /usr/local/boost_1_62_0
DEPENDPATH += /home/bjn/Uni/osm_fapra/osmpbf/osmpbf/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/release/libosmpbf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/debug/libosmpbf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/release/osmpbf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../osm_project/dependencies/osmpbf/build/osmpbf/debug/osmpbf.lib
else:unix: PRE_TARGETDEPS += /home/bjn/Uni/osm_fapra/osmpbf/osmpbf/libosmpbf.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -lz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -lz
#else:unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lz
else:unix: LIBS += usr/lib/x86_64-linux-gnu/ -lz

#INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
INCLUDEPATH += /usr/lib/x86_64-linux-gnu
#DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += /usr/lib/x86_64-linux-gnu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/release/libz.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/libz.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/release/z.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/z.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/libz.a
else:unix: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libz.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -lprotobuf
#else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lprotobuf
else:unix: LIBS += /usr/lib/x86_64-linux-gnu -lprotobuf

#INCLUDEPATH += $$PWD/../../../../usr/include/google/protobuf
INCLUDEPATH += /usr/include/google/protobuf
#DEPENDPATH += $$PWD/../../../../usr/include/google/protobuf
DEPENDPATH += /usr/include/google/protobuf

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/release/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/libprotobuf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/release/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/protobuf.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/libprotobuf.a
else:unix: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libprotobuf.a
