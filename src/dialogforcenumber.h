/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGFORCENUMBER_H
#define DIALOGFORCENUMBER_H

#include <QDialog>

namespace Ui {
class DialogForceNumber;
}

class DialogForceNumber : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForceNumber(int flag,QWidget *parent = nullptr);
    ~DialogForceNumber();

    int getFlag() const;

private slots:
    void on_pushButtonCurrentPage_clicked();

    void on_pushButtonAll_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogForceNumber *ui;
    int flag=-1;
};

#endif // DIALOGFORCENUMBER_H
