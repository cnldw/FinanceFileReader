#ifndef DIALOGSHOWTABLECOMPAREVIEW_H
#define DIALOGSHOWTABLECOMPAREVIEW_H

#include <QDialog>
#include <QStringList>
#include<QList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include<QClipboard>
#include<QPoint>
#include<QMenu>
namespace Ui {
class DialogShowTableCompareView;
}

class DialogShowTableCompareView : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableCompareView(QStringList title,QMap<int,QStringList> * compareData,QWidget *parent = 0);
    ~DialogShowTableCompareView();

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::DialogShowTableCompareView *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QPoint posCurrentMenu;
    QAction *action_ShowCopyColum;
};

#endif // DIALOGSHOWTABLECOMPAREVIEW_H
