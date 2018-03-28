#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);

    MainWindow w;
    w.show();

    return app.exec();
}
