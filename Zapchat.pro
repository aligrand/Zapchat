QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$PWD/run

SOURCES += \
    lib/database/dbman.cpp \
    lib/server-connection/serverman.cpp \
    src/chatwindow.cpp \
    src/main.cpp \
    src/auth_page.cpp \
    src/landing_page.cpp \
    src/phone_screen.cpp \
    src/roominfopanel.cpp \
    src/roomsearchpanel.cpp \
    src/roomtablepanel.cpp \
    src/signin_widget.cpp \
    src/signiu_page.cpp \
    src/signup_widget.cpp \
    src/userinfowindow.cpp

HEADERS += \
    lib/database/dbman.h \
    lib/server-connection/serverman.h \
    src/auth_page.h \
    src/chatwindow.h \
    src/landing_page.h \
    src/phone_screen.h \
    src/roominfopanel.h \
    src/roomsearchpanel.h \
    src/roomtablepanel.h \
    src/signin_widget.h \
    src/signiu_page.h \
    src/signup_widget.h \
    src/userinfowindow.h

FORMS += \
    src/auth_page.ui \
    src/chatwindow.ui \
    src/landing_page.ui \
    src/phone_screen.ui \
    src/roominfopanel.ui \
    src/roomsearchpanel.ui \
    src/roomtablepanel.ui \
    src/signin_widget.ui \
    src/signiu_page.ui \
    src/signup_widget.ui \
    src/userinfowindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
