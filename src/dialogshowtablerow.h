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
#ifndef DIALOGSHOWTABLEROW_H
#define DIALOGSHOWTABLEROW_H

#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QMessageBox>

namespace Ui {
class DialogShowTableRow;
}

class DialogShowTableRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent = nullptr);
    ~DialogShowTableRow();

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void on_pushButton_clicked();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::DialogShowTableRow *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QPoint posCurrentMenu;
    //
    int currentRow =0;
    int currentColumn=0;

    bool endFlag=false;
};

#endif // DIALOGSHOWTABLEROW_H
