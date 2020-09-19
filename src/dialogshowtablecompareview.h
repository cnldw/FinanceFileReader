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
#ifndef DIALOGSHOWTABLECOMPAREVIEW_H
#define DIALOGSHOWTABLECOMPAREVIEW_H

#include <QDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QColor>
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QDesktopServices>
#include <QDateTime>
#include <QFileDialog>
#include <QShortcut>
#include <QDebug>
#include "src/utils.h"

namespace Ui {
class DialogShowTableCompareView;
}

/**
 * @brief The DialogShowTableCompareView class 比对器
 */
class DialogShowTableCompareView : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableCompareView(QStringList title,QStringList fieldType,QMap<int,QStringList> * compareData,QWidget *parent = nullptr);
    ~DialogShowTableCompareView();

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void saveScreen();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DialogShowTableCompareView *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QAction *action_ShowSaveScreen;
    QList<int> colNoEqual;
    QPoint posCurrentMenu;

    int colSearch=0;
    //记录当前选中的行和列
    int rowcurrent=0;
    int colcurrent=0;
    //差异行背景色
    QColor backcolor;
};

#endif // DIALOGSHOWTABLECOMPAREVIEW_H
