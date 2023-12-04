/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGSHOWTABLEFIELDCHECK_H
#define DIALOGSHOWTABLEFIELDCHECK_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QPixmap>
#include <QTimer>
#include <QDesktopServices>
#include <QDateTime>
#include <QFileDialog>
#include <QShortcut>
#include "src/msgtoast.h"
#include "src/publicdefine.h"
#include "src/utils.h"
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

protected:
    void resizeEvent (QResizeEvent * event );

private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void resizeHeight();
    void copyToClipboard();

    void saveScreen();

private:
    Ui::DialogShowTableFieldCheck *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QAction *action_ShowSaveScreen;
    //
    QPoint posCurrentMenu;
    QTimer *resizeFiletimer;

    bool dataisOK=false;
};

#endif // DIALOGSHOWTABLEFIELDCHECK_H
