/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "formwebtools.h"
#include "ui_formwebtools.h"

FormWebTools::FormWebTools(QString title,QString path,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWebTools)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    webView = new miniblink;
    ui->gridLayout->addWidget(webView, 0, 0);
    //加载网页
    webView->load(path, true);
}

FormWebTools::~FormWebTools()
{
    delete ui;
}
