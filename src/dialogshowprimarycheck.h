/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGSHOWPRIMARYCHECK_H
#define DIALOGSHOWPRIMARYCHECK_H

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
#include <QKeyEvent>
#include "src/dialogmagnify.h"
#include "src/msgtoast.h"
#include "src/publicdefine.h"
#include "src/utils.h"

namespace Ui {
class DialogShowPrimaryCheck;
}

/**
 * @brief The DialogShowPrimaryCheck class 展示数据行
 */
class DialogShowPrimaryCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowPrimaryCheck(QList<primaryItem>  * primaryCheckResult,QWidget *parent = nullptr);
    ~DialogShowPrimaryCheck();

protected:
    void resizeEvent (QResizeEvent * event ) override;
    void closeEvent(QCloseEvent *event) override
    {
        event->ignore();
        //忽略事件并在主窗口中执行关闭
        emit sendSignalsClosePrimaryCheck();
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Escape) {
            event->ignore();
            //忽略事件并在主窗口中执行关闭
            emit sendSignalsClosePrimaryCheck();
        } else {
            QDialog::keyPressEvent(event);
        }
    }

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void resizeHeight();

    void copyToClipboard();

    void goToLine();

    void goToLine2();

    void comPare();

    void saveScreen();

    void saveScreen2();

    void saveScreen2ToClipboard();

    void saveScreen2ToClipboard2();

    void showMagnify();

    void on_pushButton_clicked();

    void on_tableWidget_itemSelectionChanged();

signals:
    void sendSignalsClosePrimaryCheck();
    void  sendActionFromPrimaryCheckTools(int type,int row1,int row2);

private:
    Ui::DialogShowPrimaryCheck *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_GoToLine;
    QAction *action_GoToLine2;
    QAction *action_Compare;
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

#endif // DIALOGSHOWPRIMARYCHECK_H
