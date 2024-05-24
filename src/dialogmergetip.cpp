/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogmergetip.h"
#include "ui_dialogmergetip.h"
#include "src/utils.h"

DialogMergeTip::DialogMergeTip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMergeTip)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
}

DialogMergeTip::~DialogMergeTip()
{
    delete ui;
}

void DialogMergeTip::on_pushButton_2_clicked()
{
    this->flag=false;
    this->close();
}

void DialogMergeTip::on_pushButton_clicked()
{
    this->flag=true;
    this->close();
}

bool DialogMergeTip::getMergeFlag(){
    return this->flag;
}
