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
#include "dialogaboutthis.h"
#include "ui_dialogaboutthis.h"

DialogAboutThis::DialogAboutThis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutThis)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowTitle("关于本程序");
    ui->label->setWordWrap(true);
    ui->label->setText(tr("本程序是一个可以解析格式化显示《开放式基金数据交换协议》、《中国结算开放式基金系统管理人数据接口规范》、等协议文件的工具\r\n可以用于解析如上所述协议所交换的各种文件\r\n作      者:793554262@qq.com(刘德位)\r\n商业支持:793554262@qq.com(刘德位)\r\n友情赞助:15238872101(支付宝)\r\n\r\n版      本:")+Utils::getVersion()+"\r\n编译日期:"+Utils::getCompileDate());
}

DialogAboutThis::~DialogAboutThis()
{
    delete ui;
}
