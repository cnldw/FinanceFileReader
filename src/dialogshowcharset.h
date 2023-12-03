/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
    explicit DialogShowCharset(QList<QStringList> * rowdata,QString fileencoding,QWidget *parent = 0);
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
