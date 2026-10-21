QT += core gui widgets
CONFIG += c++17
TEMPLATE = app
TARGET = MESURA

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    clientspage.cpp

HEADERS += \
    mainwindow.h \
    clientspage.h

FORMS += gestion_utilisateurs_MESURA.ui
RESOURCES += resources.qrc


