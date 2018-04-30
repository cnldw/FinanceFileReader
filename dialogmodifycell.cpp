#include "dialogmodifycell.h"
#include "ui_dialogmodifycell.h"

DialogModifyCell::DialogModifyCell(QString filedType,int filedLength,int filedDecLength,QString value,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyCell)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    ui->textEdit->setTextColor(QColor("#FF0000"));
    this->filedType=filedType;
    this->filedLength=filedLength;
    this->filedDecLength=filedDecLength;
    //初始化界面元素
    //字段类型
    if(filedType=="C"){
        ui->lineEdit->setText("C(字符型)");
    }
    else if(filedType=="A"){
        ui->lineEdit->setText("A(数字字符型)");
    }
    else if(filedType=="N"){
        ui->lineEdit->setText("N(数值型)");
    }
    else if(filedType=="TEXT"){
        ui->lineEdit->setText("TEXT(字符型)");
    }
    else{
        ui->lineEdit->setText(filedType);
    }
    //字段长度
    ui->lineEdit_2->setText(QString::number(filedLength));
    //小数长度
    ui->lineEdit_3->setText(QString::number(filedDecLength));
    //原始内容
    ui->lineEdit_4->setText(value);

}

bool DialogModifyCell::getModifyFlag(){
    return modify;
}

QString DialogModifyCell::getValueNew(){
    return ui->lineEdit_4->text();
}

DialogModifyCell::~DialogModifyCell()
{
    delete ui;
}

void DialogModifyCell::on_pushButton_2_clicked()
{
    modify=false;
    this->close();
}

void DialogModifyCell::on_pushButton_clicked()
{
    //保存时需要校验合法性
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    int length=ui->lineEdit_4->text().toLocal8Bit().length();
    //字符类的只判断长度
    if(filedType=="C"||filedType=="TEXT"){
        if(length>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
    else if(filedType=="A"){
        if(length>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
    else if(filedType=="N"){
        if(length>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
    else{
        if(length>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
}
