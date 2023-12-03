/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogforcenumber.h"
#include "ui_dialogforcenumber.h"

DialogForceNumber::DialogForceNumber(int flag,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForceNumber)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->comboBox->setCurrentIndex(flag+1);
}

DialogForceNumber::~DialogForceNumber()
{
    delete ui;
}

int DialogForceNumber::getFlag() const
{
    return flag;
}

void DialogForceNumber::on_pushButtonCurrentPage_clicked()
{
    //取消设置为-2
    this->flag=-2;
    this->close();
}

void DialogForceNumber::on_pushButtonAll_clicked()
{
    this->close();
}

void DialogForceNumber::on_comboBox_currentIndexChanged(int index)
{
    this->flag=index-1;
}
