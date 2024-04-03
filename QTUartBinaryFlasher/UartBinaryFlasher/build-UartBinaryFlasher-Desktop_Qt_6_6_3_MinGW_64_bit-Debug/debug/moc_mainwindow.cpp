/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UartBinaryFlasher/mainwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_OpenBinFile_clicked",
    "",
    "on_ReadBinFile_clicked",
    "on_CloseBinFile_clicked",
    "on_RefreshPorts_clicked",
    "on_OpenPort_clicked",
    "on_ClosePort_clicked",
    "on_SendBinary_clicked",
    "on_SendingProgress_valueChanged",
    "value",
    "on_Exit_clicked",
    "on_ShowBinaryFile_currentTextChanged",
    "currentText",
    "on_ListPorts_activated",
    "index"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[24];
    char stringdata5[24];
    char stringdata6[20];
    char stringdata7[21];
    char stringdata8[22];
    char stringdata9[32];
    char stringdata10[6];
    char stringdata11[16];
    char stringdata12[37];
    char stringdata13[12];
    char stringdata14[23];
    char stringdata15[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 22),  // "on_OpenBinFile_clicked"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 22),  // "on_ReadBinFile_clicked"
        QT_MOC_LITERAL(58, 23),  // "on_CloseBinFile_clicked"
        QT_MOC_LITERAL(82, 23),  // "on_RefreshPorts_clicked"
        QT_MOC_LITERAL(106, 19),  // "on_OpenPort_clicked"
        QT_MOC_LITERAL(126, 20),  // "on_ClosePort_clicked"
        QT_MOC_LITERAL(147, 21),  // "on_SendBinary_clicked"
        QT_MOC_LITERAL(169, 31),  // "on_SendingProgress_valueChanged"
        QT_MOC_LITERAL(201, 5),  // "value"
        QT_MOC_LITERAL(207, 15),  // "on_Exit_clicked"
        QT_MOC_LITERAL(223, 36),  // "on_ShowBinaryFile_currentText..."
        QT_MOC_LITERAL(260, 11),  // "currentText"
        QT_MOC_LITERAL(272, 22),  // "on_ListPorts_activated"
        QT_MOC_LITERAL(295, 5)   // "index"
    },
    "MainWindow",
    "on_OpenBinFile_clicked",
    "",
    "on_ReadBinFile_clicked",
    "on_CloseBinFile_clicked",
    "on_RefreshPorts_clicked",
    "on_OpenPort_clicked",
    "on_ClosePort_clicked",
    "on_SendBinary_clicked",
    "on_SendingProgress_valueChanged",
    "value",
    "on_Exit_clicked",
    "on_ShowBinaryFile_currentTextChanged",
    "currentText",
    "on_ListPorts_activated",
    "index"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    0,   83,    2, 0x08,    4 /* Private */,
       6,    0,   84,    2, 0x08,    5 /* Private */,
       7,    0,   85,    2, 0x08,    6 /* Private */,
       8,    0,   86,    2, 0x08,    7 /* Private */,
       9,    1,   87,    2, 0x08,    8 /* Private */,
      11,    0,   90,    2, 0x08,   10 /* Private */,
      12,    1,   91,    2, 0x08,   11 /* Private */,
      14,    1,   94,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_OpenBinFile_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ReadBinFile_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_CloseBinFile_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_RefreshPorts_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_OpenPort_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ClosePort_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_SendBinary_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_SendingProgress_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_Exit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ShowBinaryFile_currentTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_ListPorts_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_OpenBinFile_clicked(); break;
        case 1: _t->on_ReadBinFile_clicked(); break;
        case 2: _t->on_CloseBinFile_clicked(); break;
        case 3: _t->on_RefreshPorts_clicked(); break;
        case 4: _t->on_OpenPort_clicked(); break;
        case 5: _t->on_ClosePort_clicked(); break;
        case 6: _t->on_SendBinary_clicked(); break;
        case 7: _t->on_SendingProgress_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->on_Exit_clicked(); break;
        case 9: _t->on_ShowBinaryFile_currentTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->on_ListPorts_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
