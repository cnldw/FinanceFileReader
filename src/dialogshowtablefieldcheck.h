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
#ifndef DIALOGSHOWTABLEFIELDCHECK_H
#define DIALOGSHOWTABLEFIELDCHECK_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QClipboard>
#include <QPoint>
#include <QMenu>

namespace Ui {
class DialogShowTableFieldCheck;
}

/**
 * @brief The DialogShowTableFieldCheck class 字段检查工具
 */
class DialogShowTableFieldCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableFieldCheck(QList<QStringList> * data,QWidget *parent = nullptr);
    ~DialogShowTableFieldCheck();

private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

private:
    Ui::DialogShowTableFieldCheck *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    //
    QPoint posCurrentMenu;
};

#endif // DIALOGSHOWTABLEFIELDCHECK_H
