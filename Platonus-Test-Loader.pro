QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/authorization.cpp \
    src/internal.cpp \
    src/main.cpp \
    src/networkaccessmanager.cpp \
    src/platonustestloader.cpp \
    src/platonustestloaderapp.cpp \
    src/testbutton.cpp

HEADERS += \
    src/authorization.h \
    src/datas.h \
    src/internal.h \
    src/networkaccessmanager.h \
    src/platonustestloader.h \
    src/platonustestloaderapp.h \
    src/testbutton.h

FORMS += \
    src/authorization.ui \
    src/platonustestloader.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
