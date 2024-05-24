/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGMAGNIFY_H
#define DIALOGMAGNIFY_H

#include <QDialog>

namespace Ui {
class DialogMagnify;
}

class DialogMagnify : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMagnify(QString text,QWidget *parent = nullptr);
    ~DialogMagnify();

private slots:
    void on_pushButton_ZoomIn_clicked();

    void on_pushButton_ZoomOut_clicked();

private:
    void initStr();
    Ui::DialogMagnify *ui;
    int fontSize=64;
    QString str="";
};

#endif // DIALOGMAGNIFY_H
