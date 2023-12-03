/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGMODIFYCELL_H
#define DIALOGMODIFYCELL_H

#include <QDialog>
#include <QTextCodec>

namespace Ui {
class DialogModifyCell;
}

/**
 * @brief The DialogModifyCell class 修改
 */
class DialogModifyCell : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyCell(QString fieldType,int fieldLength,int fieldDecLength,QString value,QWidget *parent);
    ~DialogModifyCell();
    bool getModifyFlag();
    QString getValueNew();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_4_textChanged(const QString &arg1);

private:
    Ui::DialogModifyCell *ui;
    bool modify=false;
    QString fieldType="";
    int fieldLength=0;
    int fieldDecLength=0;
};

#endif // DIALOGMODIFYCELL_H
