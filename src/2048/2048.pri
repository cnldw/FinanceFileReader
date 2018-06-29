INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT       += core gui

SOURCES +=$$PWD/core/tile.cpp \
    $$PWD/core/board.cpp \
    $$PWD/core/game.cpp \
    $$PWD/gui/qgameboard.cpp \
    $$PWD/gui/qtile.cpp \
    $$PWD/gui/qresetbutton.cpp \
    $$PWD/core/observer.cpp \
    $$PWD/core/subject.cpp \
    $$PWD/gui/qgameoverwindow.cpp

HEADERS  += \
    $$PWD/core/tile.h \
    $$PWD/core/board.h \
    $$PWD/core/game.h \
    $$PWD/gui/qgameboard.h \
    $$PWD/gui/qtile.h \
    $$PWD/gui/qresetbutton.h \
    $$PWD/core/observer.h \
    $$PWD/core/subject.h \
    $$PWD/gui/qgameoverwindow.h

FORMS +=
