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

#ifndef DIALOGEDITHEADERFOOTER_H
#define DIALOGEDITHEADERFOOTER_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QDebug>
#include "src/ofdfiledefinition.h"

namespace Ui {
class DialogEditHeaderFooter;
}

class DialogEditHeaderFooter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditHeaderFooter(OFDFileDefinition *ofd,QList<QStringList> header,QList<QStringList>  footer,QWidget *parent = nullptr);
    ~DialogEditHeaderFooter();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    void sendNewHeaderAndFooter(QStringList header,QStringList footer);

private:
    Ui::DialogEditHeaderFooter *ui;
    QTableWidget * ptr_header_table;
    QTableWidget * ptr_footer_table;
};

#endif // DIALOGEDITHEADERFOOTER_H
