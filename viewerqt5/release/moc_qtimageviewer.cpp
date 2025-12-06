/****************************************************************************
** Meta object code from reading C++ file 'qtimageviewer.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qtimageviewer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtimageviewer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtImageViewer_t {
    QByteArrayData data[19];
    char stringdata0[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtImageViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtImageViewer_t qt_meta_stringdata_QtImageViewer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QtImageViewer"
QT_MOC_LITERAL(1, 14, 13), // "showImageLeft"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 6), // "Image*"
QT_MOC_LITERAL(4, 36, 3), // "img"
QT_MOC_LITERAL(5, 40, 14), // "showImageRight"
QT_MOC_LITERAL(6, 55, 8), // "openFile"
QT_MOC_LITERAL(7, 64, 9), // "clearFile"
QT_MOC_LITERAL(8, 74, 4), // "quit"
QT_MOC_LITERAL(9, 79, 16), // "combineImagesRGB"
QT_MOC_LITERAL(10, 96, 6), // "negate"
QT_MOC_LITERAL(11, 103, 9), // "negateLUT"
QT_MOC_LITERAL(12, 113, 8), // "powerlaw"
QT_MOC_LITERAL(13, 122, 11), // "powerlawLUT"
QT_MOC_LITERAL(14, 134, 6), // "linear"
QT_MOC_LITERAL(15, 141, 9), // "linearLUT"
QT_MOC_LITERAL(16, 151, 12), // "thresholding"
QT_MOC_LITERAL(17, 164, 15), // "thresholdingLUT"
QT_MOC_LITERAL(18, 180, 11) // "histogrameq"

    },
    "QtImageViewer\0showImageLeft\0\0Image*\0"
    "img\0showImageRight\0openFile\0clearFile\0"
    "quit\0combineImagesRGB\0negate\0negateLUT\0"
    "powerlaw\0powerlawLUT\0linear\0linearLUT\0"
    "thresholding\0thresholdingLUT\0histogrameq"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtImageViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x0a /* Public */,
       5,    1,   92,    2, 0x0a /* Public */,
       6,    0,   95,    2, 0x0a /* Public */,
       7,    0,   96,    2, 0x0a /* Public */,
       8,    0,   97,    2, 0x0a /* Public */,
       9,    0,   98,    2, 0x0a /* Public */,
      10,    0,   99,    2, 0x0a /* Public */,
      11,    0,  100,    2, 0x0a /* Public */,
      12,    0,  101,    2, 0x0a /* Public */,
      13,    0,  102,    2, 0x0a /* Public */,
      14,    0,  103,    2, 0x0a /* Public */,
      15,    0,  104,    2, 0x0a /* Public */,
      16,    0,  105,    2, 0x0a /* Public */,
      17,    0,  106,    2, 0x0a /* Public */,
      18,    0,  107,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtImageViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtImageViewer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->showImageLeft((*reinterpret_cast< Image*(*)>(_a[1]))); break;
        case 1: _t->showImageRight((*reinterpret_cast< Image*(*)>(_a[1]))); break;
        case 2: _t->openFile(); break;
        case 3: _t->clearFile(); break;
        case 4: _t->quit(); break;
        case 5: _t->combineImagesRGB(); break;
        case 6: _t->negate(); break;
        case 7: _t->negateLUT(); break;
        case 8: _t->powerlaw(); break;
        case 9: _t->powerlawLUT(); break;
        case 10: _t->linear(); break;
        case 11: _t->linearLUT(); break;
        case 12: _t->thresholding(); break;
        case 13: _t->thresholdingLUT(); break;
        case 14: _t->histogrameq(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QtImageViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_QtImageViewer.data,
    qt_meta_data_QtImageViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtImageViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtImageViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtImageViewer.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QtImageViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
