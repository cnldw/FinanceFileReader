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
#include "dialogchoosefieldcheckexportreport.h"
#include "ui_dialogchoosefieldcheckexportreport.h"

DialogChooseFieldCheckExportReport::DialogChooseFieldCheckExportReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseFieldCheckExportReport)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    /**调教字体差异,为了在macOS和linux上有更佳的字体表现，优化适配系统特性***/
#ifdef Q_OS_MAC
    this->setStyleSheet("font-size:13px;font-family:PingFangSC-Regular,sans-serif;");
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet("font-size:13px");
#endif
    ui->comboBox->addItem("HTML报告(*.html)");
    ui->comboBox->addItem("TXT报告(*.txt)");
    ui->comboBox->setCurrentIndex(0);
}

DialogChooseFieldCheckExportReport::~DialogChooseFieldCheckExportReport()
{
    delete ui;
}

void DialogChooseFieldCheckExportReport::on_pushButton3_clicked()
{
    int exptype=0;
    exptype=ui->comboBox->currentIndex();
    int openAtexported=0;
    if(ui->checkBox->isChecked()){
        openAtexported=1;
    }
    QMap <QString,int> config;
    config.insert("exptype",exptype);
    config.insert("openatexported",openAtexported);
    this->close();
    emit sendFieldExportConfig(config);
}

