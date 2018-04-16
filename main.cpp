#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    //加载翻译
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
}
