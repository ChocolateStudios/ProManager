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
    word/document.cpp \
    word/picture.cpp \
    word/tableofcontents.cpp \
    word/word.cpp

HEADERS += \
    application.h \
    word/document.h \
    word/picture.h \
    word/tableofcontents.h \
    word/word.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
