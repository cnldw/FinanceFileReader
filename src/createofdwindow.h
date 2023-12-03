/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef CREATEOFDWINDOW_H
#define CREATEOFDWINDOW_H

#include <QDialog>
#include <QString>
#include <QDate>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QDebug>
#include <QHash>
#include <QRegExp>
#include <QTextCodec>
#include <QTextStream>
#include <QApplication>
#include <QStandardPaths>
#include "src/utils.h"
#include "src/dialogmytip.h"

namespace Ui {
class CreateOFDWindow;
}

class CreateOFDWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreateOFDWindow(QWidget *parent = nullptr);
    ~CreateOFDWindow();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_calendarWidget_selectionChanged();

    void on_checkBox_stateChanged(int arg1);

    void on_comboBox_3_currentIndexChanged(int index);

private:
    bool copyFile(QString srcFile ,QString dstFile, bool coverFileIfExist,bool upDateTime);
    Ui::CreateOFDWindow *ui;
    QString windowTitle="OFD类文件生成器";
    //默认导出路径
    QString exppath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/OFDFile/";
    //配置文件路径
    QString configpath=Utils::getConfigPath()+"template/";
    //交易申请可用配置
    QStringList requestFileList;
    //行情可用配置
    QStringList infoFileList;
    //交易确认可用配置
    QStringList confirmFileList;
    //OFD文件体系编码
    QTextCodec *codecGb18030 = QTextCodec::codecForName("GB18030");

    bool needOK =true;

    int okType=0;
};
#endif // CREATEOFDWINDOW_H
