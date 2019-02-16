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
#include "dialogchoosefiletype.h"
#include "ui_dialogchoosefiletype.h"

DialogChooseFileType::DialogChooseFileType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseFileType)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

DialogChooseFileType::~DialogChooseFileType()
{
    delete ui;
}

void DialogChooseFileType::on_pushButtonCsv_clicked()
{
    this->setFileType(2);
    this->close();
}

void DialogChooseFileType::on_pushButtonFixed_clicked()
{
    this->setFileType(3);
    this->close();
}

int DialogChooseFileType::getFileType() const
{
    return fileType;
}

void DialogChooseFileType::setFileType(int value)
{
    fileType = value;
}
