/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGMAGNIFY_H
#define DIALOGMAGNIFY_H

#include <QDialog>
#include "src/publicdefine.h"
#include "src/utils.h"

namespace Ui {
class DialogMagnify;
}

class DialogMagnify : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMagnify(QString text,QWidget *parent = nullptr);
    ~DialogMagnify();

private:
    Ui::DialogMagnify *ui;
};

#endif // DIALOGMAGNIFY_H
