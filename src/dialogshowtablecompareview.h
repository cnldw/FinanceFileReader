/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
#include <QPixmap>
#include <QDesktopServices>
#include <QDateTime>
#include <QFileDialog>
#include <QShortcut>
#include <QDebug>

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
