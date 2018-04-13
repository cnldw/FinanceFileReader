#ifndef DIALOGSHOWTABLEROW_H
#define DIALOGSHOWTABLEROW_H

#include <QDialog>
#include<QTableWidget>
#include<QHBoxLayout>

namespace Ui {
class DialogShowTableRow;
}

class DialogShowTableRow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowTableRow(QList<QStringList> * rowdata,QWidget *parent = 0);
    ~DialogShowTableRow();
    void setData(QList<QStringList> * rowdata);

private slots:

    void on_pushButton_clicked();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    QList<QStringList> * rowdata;
    Ui::DialogShowTableRow *ui;
    QTableWidget * ptr_table;
    int beginRow =0;
    int beginColumn=0;
};

#endif // DIALOGSHOWTABLEROW_H
