#ifndef DIALOGSHOWTABLECOMPAREVIEW_H
#define DIALOGSHOWTABLECOMPAREVIEW_H

#include <QDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include<QClipboard>
#include<QPoint>
#include<QMenu>
#include<QColor>
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

    void on_pushButton_clicked();

private:
    Ui::DialogShowTableCompareView *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QMap<int,bool> colNoEqual;
    QPoint posCurrentMenu;

    int colSearch=0;
    //记录当前选中的行和列
    int rowcurrent=0;
    int colcurrent=0;
};

#endif // DIALOGSHOWTABLECOMPAREVIEW_H
