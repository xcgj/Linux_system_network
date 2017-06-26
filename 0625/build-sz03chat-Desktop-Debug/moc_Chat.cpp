/****************************************************************************
** Meta object code from reading C++ file 'Chat.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sz03chat/Chat.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Chat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Chat_t {
    QByteArrayData data[20];
    char stringdata[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Chat_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Chat_t qt_meta_stringdata_Chat = {
    {
QT_MOC_LITERAL(0, 0, 4),
QT_MOC_LITERAL(1, 5, 10),
QT_MOC_LITERAL(2, 16, 0),
QT_MOC_LITERAL(3, 17, 4),
QT_MOC_LITERAL(4, 22, 2),
QT_MOC_LITERAL(5, 25, 13),
QT_MOC_LITERAL(6, 39, 7),
QT_MOC_LITERAL(7, 47, 9),
QT_MOC_LITERAL(8, 57, 19),
QT_MOC_LITERAL(9, 77, 8),
QT_MOC_LITERAL(10, 86, 8),
QT_MOC_LITERAL(11, 95, 6),
QT_MOC_LITERAL(12, 102, 15),
QT_MOC_LITERAL(13, 118, 2),
QT_MOC_LITERAL(14, 121, 4),
QT_MOC_LITERAL(15, 126, 11),
QT_MOC_LITERAL(16, 138, 13),
QT_MOC_LITERAL(17, 152, 4),
QT_MOC_LITERAL(18, 157, 14),
QT_MOC_LITERAL(19, 172, 14)
    },
    "Chat\0sigNewUser\0\0name\0ip\0sigNewContent\0"
    "content\0boardcast\0sigTransFileRequest\0"
    "filename\0filesize\0peerid\0sigAckTransFile\0"
    "id\0port\0sigProgress\0SendFileInfo*\0"
    "info\0sigTransFinish\0onAcktransFile\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Chat[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06,
       5,    3,   54,    2, 0x06,
       8,    4,   61,    2, 0x06,
      12,    3,   70,    2, 0x06,
      15,    1,   77,    2, 0x06,
      18,    1,   80,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      19,    3,   83,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    3,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::QString,    9,   10,   11,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   13,   11,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   13,   11,   14,

       0        // eod
};

void Chat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Chat *_t = static_cast<Chat *>(_o);
        switch (_id) {
        case 0: _t->sigNewUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->sigNewContent((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->sigTransFileRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->sigAckTransFile((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->sigProgress((*reinterpret_cast< SendFileInfo*(*)>(_a[1]))); break;
        case 5: _t->sigTransFinish((*reinterpret_cast< SendFileInfo*(*)>(_a[1]))); break;
        case 6: _t->onAcktransFile((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Chat::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigNewUser)) {
                *result = 0;
            }
        }
        {
            typedef void (Chat::*_t)(QString , QString , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigNewContent)) {
                *result = 1;
            }
        }
        {
            typedef void (Chat::*_t)(QString , int , int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigTransFileRequest)) {
                *result = 2;
            }
        }
        {
            typedef void (Chat::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigAckTransFile)) {
                *result = 3;
            }
        }
        {
            typedef void (Chat::*_t)(SendFileInfo * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigProgress)) {
                *result = 4;
            }
        }
        {
            typedef void (Chat::*_t)(SendFileInfo * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Chat::sigTransFinish)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject Chat::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chat.data,
      qt_meta_data_Chat,  qt_static_metacall, 0, 0}
};


const QMetaObject *Chat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Chat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Chat.stringdata))
        return static_cast<void*>(const_cast< Chat*>(this));
    return QObject::qt_metacast(_clname);
}

int Chat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Chat::sigNewUser(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Chat::sigNewContent(QString _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Chat::sigTransFileRequest(QString _t1, int _t2, int _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Chat::sigAckTransFile(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Chat::sigProgress(SendFileInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Chat::sigTransFinish(SendFileInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
