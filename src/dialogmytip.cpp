/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogmytip.h"
#include "ui_dialogmytip.h"
#include "src/utils.h"

DialogMyTip::DialogMyTip(QString tip,QWidget *parent,bool information) :
    QDialog(parent),
    ui(new Ui::DialogMyTip)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    ui->label->setText(tip);
    ui->label->setWordWrap(true);
#ifdef Q_OS_MAC
    //修正macos下按钮怪异的表现形状
    ui->pushButton->setFixedHeight(30);
    ui->pushButton_2->setFixedHeight(30);
#endif
    //非选择题，只是展示个信息
    if(information){
        ui->pushButton->setVisible(false);
        ui->pushButton_2->setText("关闭");
    }
}

DialogMyTip::~DialogMyTip()
{
    delete ui;
}

bool DialogMyTip::getBoolFlag() const
{
    return boolFlag;
}

void DialogMyTip::on_pushButton_clicked()
{
    this->boolFlag=true;
    this->close();
}

void DialogMyTip::on_pushButton_2_clicked()
{
    this->boolFlag=false;
    this->close();
}
