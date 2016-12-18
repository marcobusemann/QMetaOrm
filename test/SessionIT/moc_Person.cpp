/****************************************************************************
** Meta object code from reading C++ file 'Person.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Person.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Person.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PersonSimple_t {
    QByteArrayData data[4];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PersonSimple_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PersonSimple_t qt_meta_stringdata_PersonSimple = {
    {
QT_MOC_LITERAL(0, 0, 12), // "PersonSimple"
QT_MOC_LITERAL(1, 13, 2), // "id"
QT_MOC_LITERAL(2, 16, 7), // "surname"
QT_MOC_LITERAL(3, 24, 4) // "name"

    },
    "PersonSimple\0id\0surname\0name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PersonSimple[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QString, 0x00095103,

       0        // eod
};

void PersonSimple::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        PersonSimple *_t = static_cast<PersonSimple *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getSurname(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        PersonSimple *_t = static_cast<PersonSimple *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setId(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setSurname(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PersonSimple::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PersonSimple.data,
      qt_meta_data_PersonSimple,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PersonSimple::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonSimple::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PersonSimple.stringdata0))
        return static_cast<void*>(const_cast< PersonSimple*>(this));
    return QObject::qt_metacast(_clname);
}

int PersonSimple::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
struct qt_meta_stringdata_Address_t {
    QByteArrayData data[5];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Address_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Address_t qt_meta_stringdata_Address = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Address"
QT_MOC_LITERAL(1, 8, 2), // "id"
QT_MOC_LITERAL(2, 11, 7), // "country"
QT_MOC_LITERAL(3, 19, 8), // "postCode"
QT_MOC_LITERAL(4, 28, 6) // "street"

    },
    "Address\0id\0country\0postCode\0street"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Address[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QString, 0x00095103,
       4, QMetaType::QString, 0x00095103,

       0        // eod
};

void Address::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        Address *_t = static_cast<Address *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getCountry(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getPostCode(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getStreet(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Address *_t = static_cast<Address *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setId(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setCountry(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setPostCode(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setStreet(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Address::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Address.data,
      qt_meta_data_Address,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Address::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Address::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Address.stringdata0))
        return static_cast<void*>(const_cast< Address*>(this));
    return QObject::qt_metacast(_clname);
}

int Address::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_PersonComplex_t {
    QByteArrayData data[6];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PersonComplex_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PersonComplex_t qt_meta_stringdata_PersonComplex = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PersonComplex"
QT_MOC_LITERAL(1, 14, 2), // "id"
QT_MOC_LITERAL(2, 17, 7), // "surname"
QT_MOC_LITERAL(3, 25, 4), // "name"
QT_MOC_LITERAL(4, 30, 7), // "address"
QT_MOC_LITERAL(5, 38, 12) // "Address::Ptr"

    },
    "PersonComplex\0id\0surname\0name\0address\0"
    "Address::Ptr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PersonComplex[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QString, 0x00095103,
       4, 0x80000000 | 5, 0x0009510b,

       0        // eod
};

void PersonComplex::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Address::Ptr >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        PersonComplex *_t = static_cast<PersonComplex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getSurname(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getName(); break;
        case 3: *reinterpret_cast< Address::Ptr*>(_v) = _t->getAddress(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        PersonComplex *_t = static_cast<PersonComplex *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setId(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setSurname(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setAddress(*reinterpret_cast< Address::Ptr*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
}

static const QMetaObject * const qt_meta_extradata_PersonComplex[] = {
        &Address::staticMetaObject,
    Q_NULLPTR
};

const QMetaObject PersonComplex::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PersonComplex.data,
      qt_meta_data_PersonComplex,  qt_static_metacall, qt_meta_extradata_PersonComplex, Q_NULLPTR}
};


const QMetaObject *PersonComplex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonComplex::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PersonComplex.stringdata0))
        return static_cast<void*>(const_cast< PersonComplex*>(this));
    return QObject::qt_metacast(_clname);
}

int PersonComplex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
   if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
