#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

//曲径通幽处，禅房花木深
//从此处---开启新世界的大门
int main(int argc, char *argv[])
{
    //启用高分辨率支持
    //仅使用QT5.6.0以及以上版本支持
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    QTranslator translator;
    //加载翻译文件,解决界面右键菜单显示为英文的问题
    //翻译文件取自Qt的translations目录版本为5.10.0
    translator.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    return app.exec();
}
