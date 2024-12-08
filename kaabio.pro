# Core Qt modules
QT       += core gui sql
QT       += widgets
QT       += sql
QT       += charts
QT       += printsupport
QT       += core gui sql network
QT       += core serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# C++ standards
CONFIG   += c++17

# Template type and target name
TEMPLATE = app
TARGET   = kaabio.pro

# Include paths
INCLUDEPATH += .

# Source files
SOURCES += main.cpp \
           connection.cpp \
           mainwindow.cpp \
          produits.cpp

# Header files
HEADERS  += connection.h \
            mainwindow.h \
            produits.h
# UI form files
FORMS    +=  mainwindow.ui



# Deployment settings
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
