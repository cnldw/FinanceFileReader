/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QStringList>

QString firstRareCharactersFontGlobal = "";
QString firstUIFontGlobal = "";
QString fontSizeStand ="";

/*********************************
*   曲径通幽处，禅房花木深         *
*   从此处，开启新世界的大门       *
*   ❤2021，愿爱与你相伴❤        *
*   ❤2022，喜得男宝一枚❤        *
*   ❤2024，马上两岁啦。❤        *
*********************************/

int main(int argc, char *argv[])
{

#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);

    MainWindow w(argc,argv);
    w.show();
    return app.exec();
}
