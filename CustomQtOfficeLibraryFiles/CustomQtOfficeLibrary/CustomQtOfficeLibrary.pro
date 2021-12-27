QT -= gui
QT += axcontainer

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    excel/excel.cpp \
    ppt/powerpoint.cpp \
    word/document.cpp \
    word/font.cpp \
    word/picture.cpp \
    word/rangeinserter.cpp \
    word/selectioninserter.cpp \
    word/table.cpp \
    word/tableofcontents.cpp \
    word/word.cpp

HEADERS += \
    application.h \
    excel/excel.h \
    ppt/powerpoint.h \
    word/document.h \
    word/font.h \
    word/inserter.h \
    word/margin.h \
    word/picture.h \
    word/rangeinserter.h \
    word/selectioninserter.h \
    word/table.h \
    word/tableofcontents.h \
    word/word.h \
    word/wordcolorfontslist.h \
    word/wordenums.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
