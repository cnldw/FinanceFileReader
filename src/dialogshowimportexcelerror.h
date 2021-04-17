#ifndef DIALOGSHOWIMPORTEXCELERROR_H
#define DIALOGSHOWIMPORTEXCELERROR_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class DialogShowImportExcelError;
}

class DialogShowImportExcelError : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowImportExcelError(QStringList*  errinfo,QWidget *parent = nullptr);
    ~DialogShowImportExcelError();

private:
    Ui::DialogShowImportExcelError *ui;
    QTableWidget * ptr_table;
};

#endif // DIALOGSHOWIMPORTEXCELERROR_H
