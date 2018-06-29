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
#ifndef DIALOGMODIFYCELL_H
#define DIALOGMODIFYCELL_H

#include <QDialog>
#include <QTextCodec>

namespace Ui {
class DialogModifyCell;
}

class DialogModifyCell : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyCell(QString filedType,int filedLength,int filedDecLength,QString value,QWidget *parent);
    ~DialogModifyCell();
    bool getModifyFlag();
    QString getValueNew();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogModifyCell *ui;
    bool modify=false;
    QString filedType="";
    int filedLength=0;
    int filedDecLength=0;
};

#endif // DIALOGMODIFYCELL_H
