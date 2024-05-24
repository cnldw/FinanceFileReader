/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
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
