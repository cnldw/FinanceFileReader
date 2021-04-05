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
