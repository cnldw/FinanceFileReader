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

private:
    QList<QStringList> * rowdata;
    Ui::DialogShowTableRow *ui;
};

#endif // DIALOGSHOWTABLEROW_H
