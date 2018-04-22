#ifndef DIALOGSHOWTABLEROW_H
#define DIALOGSHOWTABLEROW_H

#include <QDialog>
#include<QTableWidget>
#include<QHBoxLayout>
#include<QClipboard>
#include<QPoint>
#include<QMenu>
namespace Ui {
class DialogShowTableRow;
}

class DialogShowTableRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent = 0);
    ~DialogShowTableRow();

private slots:

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void copyToClipboard();

    void on_pushButton_clicked();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::DialogShowTableRow *ui;
    QTableWidget * ptr_table;
    //表格菜单
    QMenu *tablePopMenu;
    QAction *action_ShowCopyColum;
    QPoint posCurrentMenu;
    //
    int beginRow =0;
    int beginColumn=0;
};

#endif // DIALOGSHOWTABLEROW_H
