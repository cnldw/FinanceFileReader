/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef FORMWEBTOOLS_H
#define FORMWEBTOOLS_H

#include <QWidget>
#ifdef Q_OS_WIN32
#include "src/miniblink/miniblink.h"
#endif
#include "QFileInfo"


namespace Ui {
class FormWebTools;
}

class FormWebTools : public QWidget
{
    Q_OBJECT

public:
    explicit FormWebTools(QString title,QString path,QWidget *parent = nullptr);
    ~FormWebTools();

private:
    Ui::FormWebTools *ui;
#ifdef Q_OS_WIN32
    miniblink *webView;
#endif
};

#endif // FORMWEBTOOLS_H
