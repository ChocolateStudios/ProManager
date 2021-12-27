QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/analizers/tag/spreadsheettaganalizer.cpp \
    src/analizers/tag/taganalizer.cpp \
    src/analizers/tag/textdocumenttaganalizer.cpp \
    src/analizers/textanalizer.cpp \
    src/converters/excelconverter.cpp \
    src/converters/powerpointconverter.cpp \
    src/converters/wordconverter.cpp \
    src/datastream/readfilestream.cpp \
    src/datastream/savefilestream.cpp \
    src/docks/dockcontroller.cpp \
    src/docks/dockfactory.cpp \
    src/docks/views/fileexportpropertiesdock.cpp \
    src/docks/views/fileresourceimporterdock.cpp \
    src/docks/views/imageresourceimporterdock.cpp \
    src/docks/views/navegationconfigdock.cpp \
    src/docks/views/navegationdock.cpp \
    src/docks/views/stylecreatordock.cpp \
    src/docks/views/styleeditordock.cpp \
    src/exporter/exporter.cpp \
    src/editors/baseeditor.cpp \
    src/editors/spreadsheeteditor.cpp \
    src/editors/textdocumenteditor.cpp \
    src/textrange/textrange.cpp \
    src/main.cpp \
    src/maintabwidget.cpp \
    src/mainwindow.cpp \
    src/menus/stylescontextmenu.cpp \
    src/resources/fileexportpropertiesresource.cpp \
    src/resources/fileresource.cpp \
    src/resources/imageresource.cpp \
    src/resources/styleresource.cpp \
    src/spreadsheet/spreadsheet.cpp \
    src/spreadsheet/spreadsheetdelegate.cpp \
    src/textrange/textrangescontroller.cpp \
    src/widgets/popupbutton.cpp

HEADERS += \
    src/analizers/tag/spreadsheettaganalizer.h \
    src/analizers/tag/taganalizer.h \
    src/analizers/tag/textdocumenttaganalizer.h \
    src/analizers/textanalizer.h \
    src/converters/baseconverter.h \
    src/converters/excelconverter.h \
    src/converters/powerpointconverter.h \
    src/converters/wordconverter.h \
    src/datastream/readfilestream.h \
    src/datastream/savefilestream.h \
    src/docks/dockarguments.h \
    src/docks/dockfactory.h \
    src/docks/views/fileexportpropertiesdock.h \
    src/docks/views/fileresourceimporterdock.h \
    src/docks/views/imageresourceimporterdock.h \
    src/docks/views/navegationconfigdock.h \
    src/docks/views/navegationdock.h \
    src/docks/views/stylecreatordock.h \
    src/docks/views/styleeditordock.h \
    src/exporter/exporter.h \
    src/exporter/exporterdialog.h \
    src/editors/baseeditor.h \
    src/editors/spreadsheeteditor.h \
    src/editors/textdocumenteditor.h \
    src/textrange/textrange.h \
    src/docks/basedock.h \
    src/docks/dockcontroller.h \
    src/menus/stylescontextmenu.h \
    src/observers/fileresourcesobserver.h \
    src/observers/imageresourcesobserver.h \
    src/observers/observer.h \
    src/observers/styleresourcesobserver.h \
    src/observers/subjects/subject.h \
    src/resources/controllers/fileexportpropertiesresourcescontroller.h \
    src/resources/controllers/imageresourcescontroller.h \
    src/resources/controllers/resourcescontroller.h \
    src/resources/controllers/styleresourcescontroller.h \
    src/resources/fileexportpropertiesresource.h \
    src/resources/fileresource.h \
    src/resources/imageresource.h \
    src/resources/manager/resourcesmanager.h \
    src/resources/resource.h \
    src/maintabwidget.h \
    src/mainwindow.h \
    src/resource.h \
    src/resources/styleresource.h \
    src/resources/textdocumentfontstyle.h \
    src/sort/heap.h \
    src/spreadsheet/spreadsheet.h \
    src/spreadsheet/spreadsheetdelegate.h \
    src/textrange/textrangescontroller.h \
    src/widgets/popupbutton.h

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
