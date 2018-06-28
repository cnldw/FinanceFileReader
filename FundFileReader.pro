#-------------------------------------------------
#
# Project created by QtCreator 2018-03-24T10:23:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FFReader-x64
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#引入excel操作库
include(xlsx/qtxlsx.pri)
#引入2048小游戏
include(2048/2048.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    utils.cpp \
    ofdfiledefinition.cpp \
    fielddefinition.cpp \
    codeinfo.cpp \
    dictionary.cpp \
    dialogshowtablerow.cpp \
    dialogshowtablefiledcheck.cpp \
    dialogshowtablecompareview.cpp \
    dialogmodifycell.cpp \
    dialogaboutauthor.cpp \
    dialogmergetip.cpp \
    dialogaboutthis.cpp

HEADERS += \
        mainwindow.h \
    utils.h \
    ofdfiledefinition.h \
    fielddefinition.h \
    codeinfo.h \
    dictionary.h \
    dialogshowtablerow.h \
    dialogshowtablefiledcheck.h \
    dialogshowtablecompareview.h \
    dialogmodifycell.h \
    dialogaboutauthor.h \
    dialogmergetip.h \
    dialogaboutthis.h

FORMS += \
        mainwindow.ui \
    dialogshowtablerow.ui \
    dialogshowtablefiledcheck.ui \
    dialogshowtablecompareview.ui \
    dialogmodifycell.ui \
    dialogaboutauthor.ui \
    dialogmergetip.ui \
    dialogaboutthis.ui

RESOURCES += \
    res.qrc

DISTFILES +=
win32{
#windows下应用图标
RC_ICONS =res/icon.ico

#版本号
VERSION = 1.5.3
# 语言
# 0x0004 表示 简体中文
RC_LANG = 0x0004
# 产品名称
QMAKE_TARGET_PRODUCT = FFReader
# 详细描述
QMAKE_TARGET_DESCRIPTION = Fund File Reader
# 版权
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2018 Liudewei
}
macx{
#macOS下应用图标
ICON = res/icon.icns
#macoOS下自动打包配置文件到Resources目录
QMAKE_POST_LINK = cp $$PWD/config/*.* $$OUT_PWD/FFReader-x64.app/Contents/Resources/
}
