/****************************************************************************
** Meta object code from reading C++ file 'swiftapiparserzb.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "swiftapiparserzb.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swiftapiparserzb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SwiftApiParserZB_t {
    QByteArrayData data[7];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SwiftApiParserZB_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SwiftApiParserZB_t qt_meta_stringdata_SwiftApiParserZB = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SwiftApiParserZB"
QT_MOC_LITERAL(1, 17, 13), // "parseResponse"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 4), // "uuid"
QT_MOC_LITERAL(4, 37, 28), // "SwiftApiClient::AsyncMethods"
QT_MOC_LITERAL(5, 66, 6), // "method"
QT_MOC_LITERAL(6, 73, 4) // "data"

    },
    "SwiftApiParserZB\0parseResponse\0\0uuid\0"
    "SwiftApiClient::AsyncMethods\0method\0"
    "data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SwiftApiParserZB[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 4, QMetaType::QByteArray,    3,    5,    6,

       0        // eod
};

void SwiftApiParserZB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SwiftApiParserZB *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parseResponse((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const SwiftApiClient::AsyncMethods(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SwiftApiParserZB::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftApiParser::staticMetaObject>(),
    qt_meta_stringdata_SwiftApiParserZB.data,
    qt_meta_data_SwiftApiParserZB,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SwiftApiParserZB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwiftApiParserZB::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SwiftApiParserZB.stringdata0))
        return static_cast<void*>(this);
    return SwiftApiParser::qt_metacast(_clname);
}

int SwiftApiParserZB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftApiParser::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
