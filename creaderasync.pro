QT = core
QT += serialport

CONFIG += console
CONFIG -= app_bundle

TARGET = creaderasync
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 \
                  -O3 \
                  -march=native

HEADERS += \
    serialportreader.h

SOURCES += \
    main.cpp \
    serialportreader.cpp

LIBS += -lX11 \
        -lm \
        -lXtst

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/creaderasync
INSTALLS += target
