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
#include "dialogopenfileadv.h"
#include "ui_dialogopenfileadv.h"

DialogOpenFileAdv::DialogOpenFileAdv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpenFileAdv)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->comboBoxFileType->addItem("OFD文件");

}

DialogOpenFileAdv::~DialogOpenFileAdv()
{
    delete ui;
}
