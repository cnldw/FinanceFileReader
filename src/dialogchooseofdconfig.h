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

#ifndef DIALOGCHOOSEOFDCONFIG_H
#define DIALOGCHOOSEOFDCONFIG_H

#include <QDialog>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "src/ofdfiledefinition.h"

namespace Ui {
class DialogChooseOFDConfig;
}

class DialogChooseOFDConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseOFDConfig(QString ofdini,QList<OFDFileDefinition> * matchOFD,QWidget *parent = nullptr);
    ~DialogChooseOFDConfig();

    int getChooeseIndex() const;

    bool getConfirm() const;
    void setConfirm(bool value);

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DialogChooseOFDConfig *ui;
    //默认选择第一个
    int chooeseIndex=0;
    bool  confirm=false;
    QTableWidget * ptr_table;
};

#endif // DIALOGCHOOSEOFDCONFIG_H
