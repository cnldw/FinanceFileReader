/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGSHOWFIELDCHECKLIST_H
#define DIALOGSHOWFIELDCHECKLIST_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QPixmap>
#include <QDesktopServices>
#include <QDateTime>
#include <QTimer>
#include <QFileDialog>
#include <QShortcut>
#include "src/msgtoast.h"
namespace Ui {
class DialogShowFieldCheckList;
}

class DialogShowFieldCheckList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowFieldCheckList(QList<QStringList> * data,QWidget *parent = nullptr);
    ~DialogShowFieldCheckList();

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

private:
    Ui::DialogShowFieldCheckList *ui;
    QTableWidget * ptr_table;
    QTimer *resizeFiletimer;
    bool dataisOK=false;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QAction *action_ShowSaveScreen;
    QAction *action_ShowSaveScreen2ToClipboard;
    QAction *action_ShowSaveScreen2;
    QAction *action_ShowSaveScreen2ToClipboard2;
    QPoint posCurrentMenu;
    void SscreenShoot(int Type);
};

#endif // DIALOGSHOWFIELDCHECKLIST_H
