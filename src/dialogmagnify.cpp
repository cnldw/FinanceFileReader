/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogmagnify.h"
#include "ui_dialogmagnify.h"
#include "src/publicdefine.h"

DialogMagnify::DialogMagnify(QString text,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMagnify)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
#endif
#ifdef Q_OS_WIN32
    this->setStyleSheet(UIFontsWindows);
#endif
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    QString html;
    html.append("<div style=\"");
    html.append(FONTSIZE64);
    html.append("\"");
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
    html.append(text);
    html.append("</div>");
    ui->textEdit->setHtml(html);
}

DialogMagnify::~DialogMagnify()
{
    delete ui;
}
