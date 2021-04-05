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
#include "dialogchooseexporttype.h"
#include "ui_dialogchooseexporttype.h"

DialogChooseExportType::DialogChooseExportType(int currentFileType,int pageCount,int currentPage,bool rowMoreThanExcelMax,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseExportType)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    //默认导出excel无法选择编码方式
    ui->checkBox->setEnabled(false);
    //当前文件记录数超过了允许直接导出的最大行数
    if(rowMoreThanExcelMax){
        this->rowMoreThanExcelMax=rowMoreThanExcelMax;
        ui->pushButtonAll->setEnabled(false);
        ui->pushButtonAll->setToolTip("当前文件行数过多,无法一次性全部导出...");
    }
    //pagecount
    if(pageCount<2){
        ui->pushButtonCurrentPage->setEnabled(false);
        ui->pushButtonCurrentPage->setToolTip("当前文件只有一页数据,请导出全部...");
    }else{
        ui->pushButtonCurrentPage->setText("导出当前页(第"+QString::number(currentPage)+"页)");
    }
    //根据当前打开文件类型决定可以导出的文件类型
    this->currentFileType=currentFileType;
    switch (currentFileType) {
    //OFD
    case 1:
        ui->comboBox->addItem("Excel文件(*.xlsx)");
        ui->comboBox->addItem("Html文件(*.html)");
        ui->comboBox->addItem("Csv文件-竖线分割(*.csv)");
        ui->comboBox->addItem("Csv文件-制表符分割(*.csv)");
        ui->comboBox->addItem("Csv文件-逗号分割(*.csv)");
        break;
        //CSV
    case 2:
        ui->comboBox->addItem("Excel文件(*.xlsx)");
        ui->comboBox->addItem("Html文件(*.html)");
        break;
        //FIXED
    case 3:
        ui->comboBox->addItem("Excel文件(*.xlsx)");
        ui->comboBox->addItem("Html文件(*.html)");
        ui->comboBox->addItem("Csv文件-竖线分割(*.csv)");
        ui->comboBox->addItem("Csv文件-制表符分割(*.csv)");
        ui->comboBox->addItem("Csv文件-逗号分割(*.csv)");
        break;
        //DBF
    case 4:
        ui->comboBox->addItem("Excel文件(*.xlsx)");
        ui->comboBox->addItem("Html文件(*.html)");
        ui->comboBox->addItem("Csv文件-竖线分割(*.csv)");
        ui->comboBox->addItem("Csv文件-制表符分割(*.csv)");
        ui->comboBox->addItem("Csv文件-逗号分割(*.csv)");
        break;
    default:
        break;
    }
}

DialogChooseExportType::~DialogChooseExportType()
{
    delete ui;
}

void DialogChooseExportType::on_pushButtonAll_clicked()
{
    this->exportType=1;
    this->close();
}

void DialogChooseExportType::on_pushButtonCurrentPage_clicked()
{
    this->exportType=2;
    this->close();
}

/**
 * @brief DialogChooseExportType::on_comboBox_currentIndexChanged 选择不同的导出类型
 * @param index
 */
void DialogChooseExportType::on_comboBox_currentIndexChanged(int index)
{
    //仅仅导出excel文件时考虑是否禁用全部导出
    //excel
    if(index==0){
        ui->checkBox->setEnabled(false);
        exportFileType=10;
        //当前文件记录数超过了允许直接导出的最大行数
        if(rowMoreThanExcelMax){
            ui->pushButtonAll->setEnabled(false);
            ui->pushButtonAll->setToolTip("当前文件行数过多,无法一次性全部导出到Excel...");
        }
        else{
            ui->pushButtonAll->setEnabled(true);
            ui->pushButtonCurrentPage->setToolTip("");
        }
    }
    //html
    else if(index==1){
        ui->checkBox->setEnabled(true);
        exportFileType=11;
        ui->pushButtonAll->setEnabled(true);
        ui->pushButtonAll->setToolTip(NULL);
    }
    //csv-1
    else if(index==2){
        ui->checkBox->setEnabled(true);
        exportFileType=12;
        ui->pushButtonAll->setEnabled(true);
        ui->pushButtonAll->setToolTip(NULL);
    }
    //csv-2
    else if(index==3){
        ui->checkBox->setEnabled(true);
        exportFileType=13;
        ui->pushButtonAll->setEnabled(true);
        ui->pushButtonAll->setToolTip(NULL);
    }
    //csv-3
    else if(index==4){
        ui->checkBox->setEnabled(true);
        exportFileType=14;
        ui->pushButtonAll->setEnabled(true);
        ui->pushButtonAll->setToolTip(NULL);
    }
}

int DialogChooseExportType::getExportType() const
{
    return exportType;
}

int DialogChooseExportType::getExportFileType() const
{
    return exportFileType;
}

void DialogChooseExportType::on_checkBox_stateChanged(int arg1)
{
    if(arg1<1){
        this->utf8=0;
    }
    else{
        this->utf8=1;
    }
}

int DialogChooseExportType::getUtf8() const
{
    return utf8;
}
