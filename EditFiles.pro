QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    impl.cpp \
    main.cpp \
    FilesEditor.cpp

HEADERS += \
    FilesEditor.h \
    impl.h
