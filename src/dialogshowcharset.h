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
#ifndef DIALOGSHOWCHARSET_H
#define DIALOGSHOWCHARSET_H

#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QPixmap>
#include <QDesktopServices>
#include <QDateTime>
#include <QFileDialog>
#include <QShortcut>
#include "src/msgtoast.h"

namespace Ui {
class DialogShowCharset;
}

class DialogShowCharset : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowCharset(QList<QStringList> * rowdata,QWidget *parent = 0);
    ~DialogShowCharset();

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void saveScreen();

private:
    Ui::DialogShowCharset *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QAction *action_ShowSaveScreen;
    QPoint posCurrentMenu;
};

#endif // DIALOGSHOWCHARSET_H
