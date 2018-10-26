#-------------------------------------------------
#
# Project created by QtCreator 2018-03-24T10:23:52
#
#-------------------------------------------------
QT       += core gui
#要求使用的最低c++版本
CONFIG += c++11
#静态编译声明-windows,unix like
#macOS下使用动态链接库,基于macdeployqt打包即可
#macOS/Linux公用下面的配置,mac下编译如果不需要静态编译注意关闭
#在linux下编译时，如果需要静态编译，请放开下面的注释
#unix{
#    CONFIG += static
#}
#windows下静态编译开关
win32{
    CONFIG += static
}
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
include(src/xlsx/qtxlsx.pri)
#引入2048小游戏
include(src/2048/2048.pri)

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/utils.cpp \
    src/ofdfiledefinition.cpp \
    src/fielddefinition.cpp \
    src/codeinfo.cpp \
    src/dictionary.cpp \
    src/dialogshowtablerow.cpp \
    src/dialogshowtablefiledcheck.cpp \
    src/dialogshowtablecompareview.cpp \
    src/dialogmodifycell.cpp \
    src/dialogaboutauthor.cpp \
    src/dialogmergetip.cpp \
    src/dialogaboutthis.cpp \
    src/csvfielddefinition.cpp \
    src/csvfiledefinition.cpp \
    src/csvfaultcause.cpp

HEADERS += \
        src/mainwindow.h \
    src/utils.h \
    src/ofdfiledefinition.h \
    src/fielddefinition.h \
    src/codeinfo.h \
    src/dictionary.h \
    src/dialogshowtablerow.h \
    src/dialogshowtablefiledcheck.h \
    src/dialogshowtablecompareview.h \
    src/dialogmodifycell.h \
    src/dialogaboutauthor.h \
    src/dialogmergetip.h \
    src/dialogaboutthis.h \
    src/csvfielddefinition.h \
    src/csvfiledefinition.h \
    src/csvfaultcause.h

FORMS += \
        src/mainwindow.ui \
    src/dialogshowtablerow.ui \
    src/dialogshowtablefiledcheck.ui \
    src/dialogshowtablecompareview.ui \
    src/dialogmodifycell.ui \
    src/dialogaboutauthor.ui \
    src/dialogmergetip.ui \
    src/dialogaboutthis.ui

RESOURCES += \
    res.qrc \
    res.qrc

DISTFILES +=
win32{
#windows下应用图标
RC_ICONS =res/icon.ico

#版本号
VERSION = 1.5.8
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
#macoOS下自动删除上次打包产生的配置文件和打包产生的镜像后，打包配置文件到Resources目录
QMAKE_POST_LINK = rm -rf $$OUT_PWD/FFReader-x64.app/Contents/Resources/*.ini $$OUT_PWD/FFReader-x64.dmg && cp $$PWD/config/*.* $$OUT_PWD/FFReader-x64.app/Contents/Resources/
}
