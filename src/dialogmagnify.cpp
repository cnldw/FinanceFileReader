/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogmagnify.h"
#include "ui_dialogmagnify.h"
#include "src/publicdefine.h"
#include "src/utils.h"

DialogMagnify::DialogMagnify(QString text,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMagnify)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    Utils::setDefaultWindowFonts(this);
    str=text;
#ifdef Q_OS_MAC
    ui->pushButton_ZoomIn->setMinimumHeight(32);
    ui->pushButton_ZoomOut->setMinimumHeight(32);
    ui->pushButton_ZoomIn->setMaximumHeight(32);
    ui->pushButton_ZoomOut->setMaximumHeight(32);
#endif
    initStr();
}

DialogMagnify::~DialogMagnify()
{
    delete ui;
}

void DialogMagnify::on_pushButton_ZoomIn_clicked()
{
    if(fontSize>200){
        return;
    }
    else{
       fontSize+=4;
       initStr();
    }
}

void DialogMagnify::initStr(){
    QString html;
    html.append("<div style=\"font-size:");
    html.append(QString::number(fontSize));
    html.append("px;");
#ifdef Q_OS_MAC
    html.append(UIFontsMacOS);
#endif
#ifdef Q_OS_LINUX
    html.append(UIFontsLinux);
#endif
#ifdef Q_OS_WIN32
    html.append(UIFontsWindows);
#endif
    html.append("\">");
    html.append(str);
    html.append("</div>");
    ui->textEdit->setHtml(html);
}

void DialogMagnify::on_pushButton_ZoomOut_clicked()
{
    if(fontSize<20){
        return;
    }
    else{
       fontSize-=4;
       initStr();
    }
}

