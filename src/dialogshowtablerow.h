/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGSHOWTABLEROW_H
#define DIALOGSHOWTABLEROW_H

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
#include <QDebug>
#include <QTimer>
#include "src/dialogmagnify.h"
#include "src/msgtoast.h"
#include "src/publicdefine.h"
#include "src/utils.h"

namespace Ui {
class DialogShowTableRow;
}

/**
 * @brief The DialogShowTableRow class 展示数据行
 */
class DialogShowTableRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent = nullptr);
    ~DialogShowTableRow();

protected:
    void resizeEvent (QResizeEvent * event );

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void resizeHeight();

    void copyToClipboard();

    void saveScreen();

    void saveScreen2();

    void saveScreen2ToClipboard();

    void saveScreen2ToClipboard2();

    void showMagnify();

    void on_pushButton_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::DialogShowTableRow *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QAction *action_ShowSaveScreen;
    QAction *action_ShowSaveScreen2ToClipboard;
    QAction *action_ShowSaveScreen2;
    QAction *action_ShowSaveScreen2ToClipboard2;
    QAction *action_Magnify;
    QPoint posCurrentMenu;
    //
    int searchRow =0;
    int searchColumn=0;
    int maxSingleLineCharset=24;

    bool dataisOK=false;
    void SscreenShoot(int Type);
    QTimer *resizeFiletimer;
};

#endif // DIALOGSHOWTABLEROW_H
