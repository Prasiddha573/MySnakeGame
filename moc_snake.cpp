/****************************************************************************
** Meta object code from reading C++ file 'snake.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "snake.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'snake.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Snake_t {
    QByteArrayData data[13];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Snake_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Snake_t qt_meta_stringdata_Snake = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Snake"
QT_MOC_LITERAL(1, 6, 11), // "gameStarted"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 10), // "gamePaused"
QT_MOC_LITERAL(4, 30, 11), // "gameResumed"
QT_MOC_LITERAL(5, 42, 8), // "gameOver"
QT_MOC_LITERAL(6, 51, 10), // "finalScore"
QT_MOC_LITERAL(7, 62, 10), // "backToHome"
QT_MOC_LITERAL(8, 73, 8), // "gameLoop"
QT_MOC_LITERAL(9, 82, 11), // "animateHead"
QT_MOC_LITERAL(10, 94, 9), // "headScale"
QT_MOC_LITERAL(11, 104, 12), // "shadowOffset"
QT_MOC_LITERAL(12, 117, 6) // "paused"

    },
    "Snake\0gameStarted\0\0gamePaused\0gameResumed\0"
    "gameOver\0finalScore\0backToHome\0gameLoop\0"
    "animateHead\0headScale\0shadowOffset\0"
    "paused"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Snake[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       3,   58, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       7,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::Float, 0x00095103,
      11, QMetaType::Int, 0x00095103,
      12, QMetaType::Bool, 0x00095103,

       0        // eod
};

void Snake::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Snake *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->gameStarted(); break;
        case 1: _t->gamePaused(); break;
        case 2: _t->gameResumed(); break;
        case 3: _t->gameOver((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->backToHome(); break;
        case 5: _t->gameLoop(); break;
        case 6: _t->animateHead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Snake::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Snake::gameStarted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Snake::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Snake::gamePaused)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Snake::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Snake::gameResumed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Snake::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Snake::gameOver)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Snake::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Snake::backToHome)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Snake *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->headScale(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->shadowOffset(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isPaused(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<Snake *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHeadScale(*reinterpret_cast< float*>(_v)); break;
        case 1: _t->setShadowOffset(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setPaused(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject Snake::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Snake.data,
    qt_meta_data_Snake,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Snake::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Snake::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Snake.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Snake::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Snake::gameStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Snake::gamePaused()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Snake::gameResumed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Snake::gameOver(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Snake::backToHome()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
