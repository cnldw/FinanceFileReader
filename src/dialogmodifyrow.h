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
#ifndef DIALOGMODIFYROW_H
#define DIALOGMODIFYROW_H

#include <QDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QHBoxLayout>
#include <QClipboard>
#include <QPoint>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "src/ofdfiledefinition.h"

namespace Ui {
class DialogModifyRow;
}

/**
 * @brief The DialogModifyRow class 修改行
 */
class DialogModifyRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyRow(OFDFileDefinition *ofd,QStringList rowdata,QWidget *parent = nullptr);
    ~DialogModifyRow();

    QStringList getRowDataNew() const;

    bool getChange() const;

    bool getSaveFlag() const;

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_clicked();

    void on_pushButtonSave_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButtonCancal_clicked();

private:
    Ui::DialogModifyRow *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QPoint posCurrentMenu;
    //搜索开始位置
    int searchRow =0;
    //是否改变
    bool change=false;
    bool saveFlag=false;
    QStringList rowDataOld;
    //新的数据
    QStringList rowDataNew;
    //ofd
    OFDFileDefinition ofd;
    //记录错误行的hash
    QHash<int,QString> errorHash;

    void checkField(int row,int column,bool updateValue,bool displayErrorMessage);
};

#endif // DIALOGMODIFYROW_H
