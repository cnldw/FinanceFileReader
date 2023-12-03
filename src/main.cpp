/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QStringList>

//曲径通幽处，禅房花木深
//从此处---开启新世界的大门
//❤2021，愿爱与你相伴❤
//❤2022，喜得男宝一枚❤
int main(int argc, char *argv[])
{
    //启用高分辨率支持
    //仅使用QT5.6.0以及以上版本支持
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication app(argc, argv);
    QTranslator translator;
    //加载翻译文件,解决界面右键菜单显示为英文的问题
    //翻译文件取自Qt的translations目录版本为5.10.0
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);
//--暂时还是由各个界面自行控制字体设置
//#ifdef Q_OS_MAC
//    app.setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
//#endif
//#ifdef Q_OS_LINUX
//    app.setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
//#endif
//#ifdef Q_OS_WIN32
//    app.setStyleSheet(UIFontsWindows);
//#endif
    MainWindow w(argc,argv);
    w.show();
    return app.exec();
}
