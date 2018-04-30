#include "dialogmodifycell.h"
#include "ui_dialogmodifycell.h"
#include <QDebug>

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
    QString text=ui->lineEdit_4->text();
    int length=text.toLocal8Bit().length();
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
            if(text.isEmpty()){
                modify=true;
                this->close();
            }
            else{
                bool ok=true;
                for(int i=0;i<length;i++){
                    if(((int)text.at(i).toLatin1())<48||(((int)text.at(i).toLatin1())>57&&((int)text.at(i).toLatin1())<65)||(((int)text.at(i).toLatin1())>90&&((int)text.at(i).toLatin1())<97)||((int)text.at(i).toLatin1())>122){
                        ok=false;
                        break;
                    }
                }
                if(ok){
                    modify=true;
                    this->close();
                }
                else{
                    ui->textEdit->setText("数字字符型字段不建议包含0-9,a-z,A-Z外的其他字符");
                    return;
                }
            }
        }
    }
    else if(filedType=="N"){
        //空数据
        if(text.isEmpty()){
            modify=true;
            this->close();
        }
        bool ok=false;
        text.toFloat(&ok);
        if(!ok){
            ui->textEdit->setText(tr("输入的内容不是一个有效的数值"));
            return;
        }
        else{
            //无小数部分
            if(!text.contains("."))
            {
                if(length>(filedLength-filedDecLength)){
                    ui->textEdit->setText(tr("数值的整数部分长度不能超过%1").arg(filedLength-filedDecLength));
                }
                else{
                    modify=true;
                    this->close();
                }
            }
            //如果包含小数点
            else{
                //拆分出来整数和小数
                //整数部分
                int index=text.indexOf(".");
                QString intS=text.mid(0,index);
                if(intS.length()>(filedLength-filedDecLength)){
                    ui->textEdit->setText(tr("数值的整数部分长度不能超过%1").arg(QString::number(filedLength-filedDecLength)));
                    return;
                }
                QString intD=text.mid(index+1,-1);
                if(intD.length()>filedDecLength){
                    ui->textEdit->setText(tr("数值的小数部分长度不能超过%1").arg(QString::number(filedDecLength)));
                    return;
                }
                //如果没问题就好说
                modify=true;
                this->close();
            }
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
