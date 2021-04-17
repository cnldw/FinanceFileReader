/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#include "dialogmytip.h"
#include "ui_dialogmytip.h"

DialogMyTip::DialogMyTip(QString tip,QWidget *parent,bool information) :
    QDialog(parent),
    ui(new Ui::DialogMyTip)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
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
