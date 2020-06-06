/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
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
#include <QMessageBox>
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

private:
    bool copyFile(QString srcFile ,QString dstFile, bool coverFileIfExist,bool upDateTime);
    Ui::CreateOFDWindow *ui;
    QString windowTitle="OFD类文件生成器";
    //默认导出路径
    QString exppath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/OFDFile/";
    //配置文件路径
    QString configpath=Utils::getConfigPath()+"templete/";
    //交易申请可用配置
    QStringList requestFileList;
    //行情可用配置
    QStringList infoFileList;
    //交易确认可用配置
    QStringList confirmFileList;
    //OFD文件体系编码
    QTextCodec *codecGb18030 = QTextCodec::codecForName("GB18030");
};
#endif // CREATEOFDWINDOW_H
