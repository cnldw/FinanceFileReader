/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGMODIFYMTIME_H
#define DIALOGMODIFYMTIME_H

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

namespace Ui {
class DialogModifyMtime;
}

class DialogModifyMtime : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyMtime(QWidget *parent = 0);
    ~DialogModifyMtime();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void checkSelect();

    void uncheckSelect();

protected:
    //文件拖拽支持
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::DialogModifyMtime *ui;
    QTableWidget * ptr_table;
    QStringList fileList;
    //表格的右键菜单
    QMenu *tablePopMenu;
    QAction *action_Check;
    QAction *action_UnCheck;
};

#endif // DIALOGMODIFYMTIME_H
