#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
    //启用高分辨率支持
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    QTranslator translator;
    //加载翻译
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
}
