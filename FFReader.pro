#-------------------------------------------------
#
# Project created by QtCreator 2018-03-24T10:23:52
#
#-------------------------------------------------
QT       += core gui concurrent
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

#根据位数设置target名称
contains(QT_ARCH, i386) {
    TARGET = FFReader-x86
}
contains(QT_ARCH, x86_64) {
    TARGET = FFReader-x64
}

RESOURCES += \
    res.qrc

DISTFILES +=
win32{
#windows下应用图标
RC_ICONS =res/icon.ico
############以下注意按版本修改###############
#版本号-给windows生成到程序包使用
VERSION = 1.9.16
############以下注意按版本修改###############

# 语言
# 0x0004 表示 简体中文
RC_LANG = 0x0004
# 产品名称
QMAKE_TARGET_PRODUCT = FFReader
# 详细描述
QMAKE_TARGET_DESCRIPTION = Finance File Reader
# 版权
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2018 Liudewei
}
macx{
#macOS下应用图标
ICON = res/icon.icns
#macoOS下自动删除上次打包产生的配置文件和打包产生的镜像后，打包配置文件到Resources目录
QMAKE_POST_LINK =cp -rf $$PWD/config/* $$OUT_PWD/FFReader-x64.app/Contents/Resources/
}

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

#引入excel操作库-切换到新版本
#include(src/xlsx/qtxlsx.pri)
include(src/Qxlsx/QXlsx.pri)
#引入2048小游戏
include(src/2048/2048.pri)
#引入编码检测库
include(src/libucd/libucd.pri)
#引入qrcode
include(src/qrcode/Qrcode.pri)
#引入qdbf
include(src/qdbf/qdbf.pri)
#引入语法高亮库
include(src/qsourcehighlite/QSourceHighlite.pri)
#引入minibink模块,引入浏览器窗口组件,仅支持windows
win32{
    include (src/miniblink/miniblink.pri)
    SOURCES += src/formwebtools.cpp
    HEADERS += src/formwebtools.h
    FORMS += src/formwebtools.ui
}

SOURCES += \
    src/dbffielddefinition.cpp \
    src/dbffileconfig.cpp \
    src/dbffiledefinition.cpp \
    src/dialogchoosedbfconfig.cpp \
    src/dialogchooseofdconfig.cpp \
    src/dialogeditheaderfooter.cpp \
    src/dialogforcenumber.cpp \
    src/dialogmagnify.cpp \
    src/dialogoktools.cpp \
    src/dialogshareqrcode.cpp \
    src/dialogshowimportexcelerror.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
    src/utils.cpp \
    src/ofdfiledefinition.cpp \
    src/dictionary.cpp \
    src/dialogshowtablerow.cpp \
    src/dialogshowtablecompareview.cpp \
    src/dialogmodifycell.cpp \
    src/dialogaboutauthor.cpp \
    src/dialogmergetip.cpp \
    src/dialogaboutthis.cpp \
    src/csvfielddefinition.cpp \
    src/csvfiledefinition.cpp \
    src/fieldisnumber.cpp \
    src/fixedfielddefinition.cpp \
    src/ofdfielddefinition.cpp \
    src/ofdcodeinfo.cpp \
    src/fixedfiledefinition.cpp \
    src/faultcause.cpp \
    src/dialogchoosefiletype.cpp \
    src/dialogpreference.cpp \
    src/dialogmytip.cpp \
    src/dialogmodifyrow.cpp \
    src/dialogshowtablefieldcheck.cpp \
    src/ofdfaultcause.cpp \
    src/createofdwindow.cpp \
    src/dialogchooseexporttype.cpp \
    src/dialogshowcharset.cpp \
    src/dialogmodifymtime.cpp

HEADERS += \
    src/dbffielddefinition.h \
    src/dbffileconfig.h \
    src/dbffiledefinition.h \
    src/dialogchoosedbfconfig.h \
    src/dialogchooseofdconfig.h \
    src/dialogeditheaderfooter.h \
    src/dialogforcenumber.h \
    src/dialogmagnify.h \
    src/dialogoktools.h \
    src/dialogshareqrcode.h \
    src/dialogshowimportexcelerror.h \
        src/mainwindow.h \
    src/ucdutils.h \
    src/utils.h \
    src/ofdfiledefinition.h \
    src/dictionary.h \
    src/dialogshowtablerow.h \
    src/dialogshowtablecompareview.h \
    src/dialogmodifycell.h \
    src/dialogaboutauthor.h \
    src/dialogmergetip.h \
    src/dialogaboutthis.h \
    src/csvfielddefinition.h \
    src/csvfiledefinition.h \
    src/fieldisnumber.h \
    src/fixedfielddefinition.h \
    src/ofdcodeinfo.h \
    src/ofdfielddefinition.h \
    src/fixedfiledefinition.h \
    src/faultcause.h \
    src/dialogchoosefiletype.h \
    src/dialogpreference.h \
    src/dialogmytip.h \
    src/dialogmodifyrow.h \
    src/publicdefine.h \
    src/dialogshowtablefieldcheck.h \
    src/ofdfaultcause.h \
    src/createofdwindow.h \
    src/dialogchooseexporttype.h \
    src/dialogshowcharset.h \
    src/dialogmodifymtime.h

FORMS += \
    src/dialogchoosedbfconfig.ui \
    src/dialogchooseofdconfig.ui \
    src/dialogeditheaderfooter.ui \
    src/dialogforcenumber.ui \
    src/dialogmagnify.ui \
    src/dialogoktools.ui \
    src/dialogshareqrcode.ui \
    src/dialogshowimportexcelerror.ui \
        src/mainwindow.ui \
    src/dialogshowtablerow.ui \
    src/dialogshowtablecompareview.ui \
    src/dialogmodifycell.ui \
    src/dialogaboutauthor.ui \
    src/dialogmergetip.ui \
    src/dialogaboutthis.ui \
    src/dialogchoosefiletype.ui \
    src/dialogpreference.ui \
    src/dialogmytip.ui \
    src/dialogmodifyrow.ui \
    src/dialogshowtablefieldcheck.ui \
    src/createofdwindow.ui \
    src/dialogchooseexporttype.ui \
    src/dialogshowcharset.ui \
    src/dialogmodifymtime.ui
