/****************************************************************************
** Meta object code from reading C++ file 'test.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../test.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Test_t {
    QByteArrayData data[19];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Test_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Test_t qt_meta_stringdata_Test = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Test"
QT_MOC_LITERAL(1, 5, 25), // "ProcessTableDoubleClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 10), // "HandleSave"
QT_MOC_LITERAL(4, 43, 11), // "HandleStart"
QT_MOC_LITERAL(5, 55, 9), // "HandleEnd"
QT_MOC_LITERAL(6, 65, 12), // "HandleFilter"
QT_MOC_LITERAL(7, 78, 18), // "HandleRefreshTable"
QT_MOC_LITERAL(8, 97, 23), // "ProviderCheckBoxClicked"
QT_MOC_LITERAL(9, 121, 5), // "state"
QT_MOC_LITERAL(10, 127, 23), // "ProcessTableItemClicked"
QT_MOC_LITERAL(11, 151, 3), // "row"
QT_MOC_LITERAL(12, 155, 19), // "SeclectAllProviders"
QT_MOC_LITERAL(13, 175, 25), // "SeclectAllFiltedProviders"
QT_MOC_LITERAL(14, 201, 22), // "ShowSeclectedProviders"
QT_MOC_LITERAL(15, 224, 5), // "Timer"
QT_MOC_LITERAL(16, 230, 10), // "CloseEvent"
QT_MOC_LITERAL(17, 241, 12), // "QCloseEvent*"
QT_MOC_LITERAL(18, 254, 5) // "event"

    },
    "Test\0ProcessTableDoubleClicked\0\0"
    "HandleSave\0HandleStart\0HandleEnd\0"
    "HandleFilter\0HandleRefreshTable\0"
    "ProviderCheckBoxClicked\0state\0"
    "ProcessTableItemClicked\0row\0"
    "SeclectAllProviders\0SeclectAllFiltedProviders\0"
    "ShowSeclectedProviders\0Timer\0CloseEvent\0"
    "QCloseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Test[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    1,   85,    2, 0x08 /* Private */,
      10,    1,   88,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      13,    1,   94,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    1,   98,    2, 0x08 /* Private */,
      16,    1,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void Test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Test *_t = static_cast<Test *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ProcessTableDoubleClicked(); break;
        case 1: _t->HandleSave(); break;
        case 2: _t->HandleStart(); break;
        case 3: _t->HandleEnd(); break;
        case 4: _t->HandleFilter(); break;
        case 5: _t->HandleRefreshTable(); break;
        case 6: _t->ProviderCheckBoxClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ProcessTableItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->SeclectAllProviders((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->SeclectAllFiltedProviders((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->ShowSeclectedProviders(); break;
        case 11: _t->Timer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->CloseEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Test::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Test.data,
      qt_meta_data_Test,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Test::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Test.stringdata0))
        return static_cast<void*>(const_cast< Test*>(this));
    return QWidget::qt_metacast(_clname);
}

int Test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
