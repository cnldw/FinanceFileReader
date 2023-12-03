/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGABOUTTHIS_H
#define DIALOGABOUTTHIS_H

#include <QDialog>
#include "src/utils.h"
#include <QByteArray>
#include "src/publicdefine.h"

namespace Ui {
class DialogAboutThis;
}

/**
 * @brief The DialogAboutThis class 关于本软件
 */
class DialogAboutThis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutThis(QWidget *parent = nullptr);
    ~DialogAboutThis();

private:
    Ui::DialogAboutThis *ui;
};

#endif // DIALOGABOUTTHIS_H
