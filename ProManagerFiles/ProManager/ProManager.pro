QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/customs/customfile.cpp \
    src/customs/customfilepushbutton.cpp \
    src/customs/custompopupbutton.cpp \
    src/docks/dockcontroller.cpp \
    src/docks/filesdock.cpp \
    src/docks/navegationconfigdock.cpp \
    src/docks/navegationdock.cpp \
    src/extras/resourcescontroller.cpp \
    src/extras/specialtexttag.cpp \
    src/extras/textanalizer.cpp \
    src/extras/style.cpp \
    src/docks/editstyledock.cpp \
    src/docks/imagesdock.cpp \
    src/docks/createstyledock.cpp \
    src/customs/customimage.cpp \
    src/customs/customimagepushbutton.cpp \
    src/customs/customaction.cpp \
    src/customs/customcolorfontdialog.cpp \
    src/customs/customstylescontextmenu.cpp \
    src/customs/customtextedit.cpp \
    src/main.cpp \
    src/mainwindow.cpp \

HEADERS += \
    src/customs/customfile.h \
    src/customs/customfilepushbutton.h \
    src/customs/custompopupbutton.h \
    src/docks/filesdock.h \
    src/docks/navegationconfigdock.h \
    src/docks/navegationdock.h \
    src/extras/resourcescontroller.h \
    src/extras/specialtexttag.h \
    src/extras/textanalizer.h \
    src/extras/style.h \
    src/docks/basedock.h \
    src/docks/dockcontroller.h \
    src/docks/editstyledock.h \
    src/docks/imagesdock.h \
    src/docks/createstyledock.h \
    src/customs/customimage.h \
    src/customs/customimagepushbutton.h \
    src/customs/customaction.h \
    src/customs/customcolorfontdialog.h \
    src/customs/customstylescontextmenu.h \
    src/customs/customtextedit.h \
    src/mainwindow.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ProManager.qrc

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/../../CustomQtOfficeLibraryFiles/CustomQtOfficeLibrary

DEPENDPATH += $$PWD/../../CustomQtOfficeLibraryFiles/CustomQtOfficeLibrary

LIBS += \
#    -L$$PWD/../../CustomQtOfficeLibraryFiles/build-CustomQtOfficeLibrary-Desktop_Qt_6_1_2_MSVC2019_64bit-Debug/debug/ -lCustomQtOfficeLibrary \
    -L$$PWD/../../CustomQtOfficeLibraryFiles/build-CustomQtOfficeLibrary-Desktop_Qt_6_2_1_MinGW_64_bit-Debug/debug/ -lCustomQtOfficeLibrary
