/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGMERGETIP_H
#define DIALOGMERGETIP_H

#include <QDialog>

namespace Ui {
class DialogMergeTip;
}

/**
 * @brief The DialogMergeTip class 合并提示
 */
class DialogMergeTip : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMergeTip(QWidget *parent = 0);
    bool getMergeFlag();
    ~DialogMergeTip();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogMergeTip *ui;
    bool flag=false;
};

#endif // DIALOGMERGETIP_H
