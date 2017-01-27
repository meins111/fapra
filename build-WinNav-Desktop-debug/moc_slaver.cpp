/****************************************************************************
** Meta object code from reading C++ file 'slaver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WinNav/Utils/slaver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slaver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Slaver_t {
    QByteArrayData data[8];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Slaver_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Slaver_t qt_meta_stringdata_Slaver = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Slaver"
QT_MOC_LITERAL(1, 7, 13), // "parseProgress"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 15), // "percentProgress"
QT_MOC_LITERAL(4, 38, 11), // "parsingDone"
QT_MOC_LITERAL(5, 50, 11), // "successFlag"
QT_MOC_LITERAL(6, 62, 19), // "pathfindingProgress"
QT_MOC_LITERAL(7, 82, 15) // "pathfindingDone"

    },
    "Slaver\0parseProgress\0\0percentProgress\0"
    "parsingDone\0successFlag\0pathfindingProgress\0"
    "pathfindingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Slaver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,
       7,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void Slaver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Slaver *_t = static_cast<Slaver *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parseProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->parsingDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->pathfindingProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->pathfindingDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Slaver::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Slaver::parseProgress)) {
                *result = 0;
            }
        }
        {
            typedef void (Slaver::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Slaver::parsingDone)) {
                *result = 1;
            }
        }
        {
            typedef void (Slaver::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Slaver::pathfindingProgress)) {
                *result = 2;
            }
        }
        {
            typedef void (Slaver::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Slaver::pathfindingDone)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject Slaver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Slaver.data,
      qt_meta_data_Slaver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Slaver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Slaver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Slaver.stringdata0))
        return static_cast<void*>(const_cast< Slaver*>(this));
    return QObject::qt_metacast(_clname);
}

int Slaver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Slaver::parseProgress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Slaver::parsingDone(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Slaver::pathfindingProgress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Slaver::pathfindingDone(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
