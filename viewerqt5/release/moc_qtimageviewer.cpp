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
    QByteArrayData data[23];
    char stringdata0[238];
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
QT_MOC_LITERAL(3, 29, 15), // "Image<uint8_t>*"
QT_MOC_LITERAL(4, 45, 3), // "img"
QT_MOC_LITERAL(5, 49, 14), // "showImageRight"
QT_MOC_LITERAL(6, 64, 8), // "openFile"
QT_MOC_LITERAL(7, 73, 9), // "clearFile"
QT_MOC_LITERAL(8, 83, 4), // "quit"
QT_MOC_LITERAL(9, 88, 16), // "combineImagesRGB"
QT_MOC_LITERAL(10, 105, 11), // "togreyscale"
QT_MOC_LITERAL(11, 117, 6), // "negate"
QT_MOC_LITERAL(12, 124, 9), // "negateLUT"
QT_MOC_LITERAL(13, 134, 8), // "powerlaw"
QT_MOC_LITERAL(14, 143, 11), // "powerlawLUT"
QT_MOC_LITERAL(15, 155, 6), // "linear"
QT_MOC_LITERAL(16, 162, 9), // "linearLUT"
QT_MOC_LITERAL(17, 172, 12), // "thresholding"
QT_MOC_LITERAL(18, 185, 15), // "thresholdingLUT"
QT_MOC_LITERAL(19, 201, 11), // "histogrameq"
QT_MOC_LITERAL(20, 213, 7), // "lowpass"
QT_MOC_LITERAL(21, 221, 6), // "median"
QT_MOC_LITERAL(22, 228, 9) // "laplacian"

    },
    "QtImageViewer\0showImageLeft\0\0"
    "Image<uint8_t>*\0img\0showImageRight\0"
    "openFile\0clearFile\0quit\0combineImagesRGB\0"
    "togreyscale\0negate\0negateLUT\0powerlaw\0"
    "powerlawLUT\0linear\0linearLUT\0thresholding\0"
    "thresholdingLUT\0histogrameq\0lowpass\0"
    "median\0laplacian"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtImageViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x0a /* Public */,
       5,    1,  112,    2, 0x0a /* Public */,
       6,    0,  115,    2, 0x0a /* Public */,
       7,    0,  116,    2, 0x0a /* Public */,
       8,    0,  117,    2, 0x0a /* Public */,
       9,    0,  118,    2, 0x0a /* Public */,
      10,    0,  119,    2, 0x0a /* Public */,
      11,    0,  120,    2, 0x0a /* Public */,
      12,    0,  121,    2, 0x0a /* Public */,
      13,    0,  122,    2, 0x0a /* Public */,
      14,    0,  123,    2, 0x0a /* Public */,
      15,    0,  124,    2, 0x0a /* Public */,
      16,    0,  125,    2, 0x0a /* Public */,
      17,    0,  126,    2, 0x0a /* Public */,
      18,    0,  127,    2, 0x0a /* Public */,
      19,    0,  128,    2, 0x0a /* Public */,
      20,    0,  129,    2, 0x0a /* Public */,
      21,    0,  130,    2, 0x0a /* Public */,
      22,    0,  131,    2, 0x0a /* Public */,

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
        case 0: _t->showImageLeft((*reinterpret_cast< Image<uint8_t>*(*)>(_a[1]))); break;
        case 1: _t->showImageRight((*reinterpret_cast< Image<uint8_t>*(*)>(_a[1]))); break;
        case 2: _t->openFile(); break;
        case 3: _t->clearFile(); break;
        case 4: _t->quit(); break;
        case 5: _t->combineImagesRGB(); break;
        case 6: _t->togreyscale(); break;
        case 7: _t->negate(); break;
        case 8: _t->negateLUT(); break;
        case 9: _t->powerlaw(); break;
        case 10: _t->powerlawLUT(); break;
        case 11: _t->linear(); break;
        case 12: _t->linearLUT(); break;
        case 13: _t->thresholding(); break;
        case 14: _t->thresholdingLUT(); break;
        case 15: _t->histogrameq(); break;
        case 16: _t->lowpass(); break;
        case 17: _t->median(); break;
        case 18: _t->laplacian(); break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
