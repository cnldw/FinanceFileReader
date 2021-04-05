
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

#ifndef DIALOGOKTOOLS_H
#define DIALOGOKTOOLS_H

#include <QDialog>
#include <QStringList>
#include <QList>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTableWidget>
#include <QCheckBox>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QTableWidgetSelectionRange>
#include <src/utils.h>
#include <QDesktopServices>

namespace Ui {
class DialogOKTools;
}

class DialogOKTools : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOKTools(QWidget *parent = nullptr);
    ~DialogOKTools();

private slots:

    void on_pushButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void checkSelect();

    void uncheckSelect();
    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

protected:
    //文件拖拽支持
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::DialogOKTools *ui;
    QTableWidget * ptr_table;
    QStringList fileList;
    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_Check;
    QAction *action_UnCheck;

    int taskType=0;
    int okType=0;
};

#endif // DIALOGOKTOOLS_H
