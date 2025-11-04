TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include/

LIBS += -lraylib
SOURCES += \
        ModeEdition.c \
        Nouveau.c

DISTFILES += \
    build/Qt_6_4_2_qt6-Debug/config.txt \
    build/Qt_6_4_2_qt6-Debug/config.txt \
    build/Qt_6_4_2_qt6-Debug/fusée.txt \
    config.txt
