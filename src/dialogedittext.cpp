/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogedittext.h"
#include "ui_dialogedittext.h"

DialogEditText::DialogEditText(QString title,QStringList fieldList,int importType,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditText)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    if(title.isEmpty()){
        this->setWindowTitle("文本编辑");
    }
    else{
        this->setWindowTitle(title);
    }
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label->setPalette(pe);

    this->fieldList=fieldList;
    this->importType=importType;
    ui->textBrowser->setReadOnly(false);
    if(importType==0){
        ui->label->setText("你可以填写形如[字段1=条件值1&字段2=条件值2]的条件规则");
    }
    else if (importType==1){
        ui->label->setText("你可以填写形如[字段1,字段2]使用英文逗号分割的字段列表");
    }
}

DialogEditText::~DialogEditText()
{
    delete ui;
}

void DialogEditText::on_pushButton_clicked()
{
    if(ui->textBrowser->toPlainText().isEmpty()){
        ui->label->setText("没有填写任何数据,如放弃导入,请点击取消按钮...");
    }
    else{
        bool clearOldData=true;
        if(ui->radioButton_clear->isChecked()){
            clearOldData=true;
        }
        else if(ui->radioButton_merge->isChecked()){
            clearOldData=false;
        }
        if(importType==0){
            QStringList checkList=ui->textBrowser->toPlainText().split("&");
            for(int i=0;i<checkList.count();i++){
                QString item=checkList.at(i);
                if(!item.contains("=")){
                    ui->label->setText("填写的内容中的["+item+"]无法被识别成一个有效的条件...");
                    return;
                }
                else{
                    QStringList itemList=item.split("=");
                    if(itemList.isEmpty()){
                        ui->label->setText("填写的内容中貌似有连续的&符号,无法正确分割条件列表...");
                        return;
                    }
                    if(itemList.count()!=2){
                        ui->label->setText("填写的内容中的["+item+"]无法被识别成一个有效的条件...");
                        return;
                    }
                    if(itemList.at(0).isEmpty()||itemList.at(1).isEmpty()){
                        ui->label->setText("填写的内容中的["+item+"]无法被识别成一个有效的条件...");
                        return;
                    }
                    if(!fieldList.contains(itemList.at(0))){
                        ui->label->setText("填写的内容中的字段名["+itemList.at(0)+"]并不存在于此接口,请核实...");
                        return;
                    }
                }
            }
            //通过检查允许发送信号
            emit sendImportString(ui->textBrowser->toPlainText(),importType,clearOldData);
            this->close();
        }
        else if (importType==1){
            QStringList checkFieldList=ui->textBrowser->toPlainText().split(",");
            for(int i=0;i<checkFieldList.count();i++){
                QString item=checkFieldList.at(i);
                if(item.isEmpty()){
                    ui->label->setText("填写的内容中貌似有连续的,符号,无法正确分割字段列表...");
                    return;
                }
                if(!fieldList.contains(item)){
                    ui->label->setText("填写的内容中的字段名["+item+"]并不存在于此接口,请核实...");
                    return;
                }
            }
            //通过检查允许发送信号
            emit sendImportString(ui->textBrowser->toPlainText(),importType,clearOldData);
            this->close();
        }
    }
}


void DialogEditText::on_pushButton_2_clicked()
{
    this->close();
}

