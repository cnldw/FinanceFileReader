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
#ifndef DIALOGCHOOSEEXPORTTYPE_H
#define DIALOGCHOOSEEXPORTTYPE_H

#include <QDialog>

namespace Ui {
class DialogChooseExportType;
}

class DialogChooseExportType : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseExportType(int currentFileType,int pageCount,int currentPage,bool rowMoreThanExcelMax,QWidget *parent = 0);
    ~DialogChooseExportType();

    int getExportFileType() const;

    int getExportType() const;

    int getUtf8() const;

private slots:
    void on_pushButtonAll_clicked();

    void on_pushButtonCurrentPage_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::DialogChooseExportType *ui;
    bool rowMoreThanExcelMax=false;
    //0,直接关闭，1导出整个文件，2导出当前页
    int exportType=0;
    //当前打开的文件类型
    int currentFileType;
    //导出文件类型10:excel,11:html,12:Csv文件-竖线分割,13:Csv文件-制表符分割,14：Csv文件-逗号分割
    int exportFileType=10;
    //是否使用utf-8导出
    int utf8=0;
};

#endif // DIALOGCHOOSEEXPORTTYPE_H
