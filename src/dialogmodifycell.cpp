/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/dialogmodifycell.h"
#include "ui_dialogmodifycell.h"
#include "src/publicdefine.h"

DialogModifyCell::DialogModifyCell(QString fieldType,int fieldLength,int fieldDecLength,QString value,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyCell)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
#endif
#ifdef Q_OS_WIN32
    this->setStyleSheet(UIFontsWindows);
#endif
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->textEdit->setTextColor(QColor("#FF0000"));
    this->fieldType=fieldType;
    this->fieldLength=fieldLength;
    this->fieldDecLength=fieldDecLength;
    ui->lineEdit_4->setFocus();
    //初始化界面元素
    //字段类型
    if(fieldType=="C"){
        ui->lineEdit->setText("C(字符型)");
    }
    else if(fieldType=="A"){
        ui->lineEdit->setText("A(数字字符型)");
    }
    else if(fieldType=="N"){
        ui->lineEdit->setText("N(数值型)");
    }
    else if(fieldType=="TEXT"){
        ui->lineEdit->setText("TEXT(文本型)");
    }
    else{
        ui->lineEdit->setText(fieldType);
    }
    //字段长度
    ui->lineEdit_2->setText(QString::number(fieldLength));
    //小数长度
    ui->lineEdit_3->setText(QString::number(fieldDecLength));
    //原始内容
    ui->lineEdit_4->setText(value);
}

bool DialogModifyCell::getModifyFlag(){
    return modify;
}

QString DialogModifyCell::getValueNew(){
    if(fieldType=="N"){
        return ui->lineEdit_4->text().replace(",","");
    }
    else{
        return ui->lineEdit_4->text();
    }
}

DialogModifyCell::~DialogModifyCell()
{
    delete ui;
}

//取消保存
void DialogModifyCell::on_pushButton_2_clicked()
{
    modify=false;
    this->close();
}

//保存
void DialogModifyCell::on_pushButton_clicked()
{
    //保存时需要校验合法性
    QTextCodec *codec=QTextCodec::codecForName("GB18030");
    //内容
    QString text=ui->lineEdit_4->text();
    //长度
    int textLength=codec->fromUnicode(text).length();
    //字符类和文本类的只需要判断长度
    if(fieldType=="C"||fieldType=="TEXT"){
        //长度校验
        if(textLength>fieldLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(fieldLength));
        }
        else{
            modify=true;
            this->close();
        }
    }
    //数字字符型
    else if(fieldType=="A"){
        //长度校验
        if(textLength>fieldLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(fieldLength));
        }
        else{
            //内容校验-空
            if(text.isEmpty()){
                modify=true;
                this->close();
            }
            //内容校验非空
            else{
                //非空时仅允许出现数字和字母且不准以空格开头
                if(text.at(0)==' '){
                    ui->textEdit->setText("数字字符型字段不建议以空格开头请修正");
                    return;
                }
                else{
                    bool ok=true;
                    QString textTrimed=text.trimmed();
                    for(int i=0;i<textTrimed.length();i++){
                        if(((int)textTrimed.at(i).toLatin1())<48||(((int)textTrimed.at(i).toLatin1())>57&&((int)textTrimed.at(i).toLatin1())<65)||(((int)textTrimed.at(i).toLatin1())>90&&((int)textTrimed.at(i).toLatin1())<97)||((int)textTrimed.at(i).toLatin1())>122){
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
                    }
                }
            }
        }
    }
    //数字型
    else if(fieldType=="N"){
        //如果是千位分隔符
        if(text.contains(",")){
            text=text.replace(",","");
        }
        //空数据,空数据由主窗口设置为全0
        if(text.isEmpty()){
            modify=true;
            this->close();
        }
        bool ok=false;
        text.toDouble(&ok);
        //如果无法转化为Double直接认为不合规//兼容负数
        if(!ok){
            ui->textEdit->setText(tr("输入的内容不是一个有效的数值"));
        }
        else{
            //无小数部分
            if(!text.contains("."))
            {
                if(textLength>(fieldLength-fieldDecLength)){
                    ui->textEdit->setText(tr("数值的整数部分长度不能超过%1").arg(fieldLength-fieldDecLength));
                }
                else{
                    modify=true;
                    this->close();
                }
            }
            //如果包含小数点
            else{
                //拆分出来整数和小数
                int index=text.indexOf(".");
                //整数部分
                QString intS=text.mid(0,index);
                if(intS.length()>(fieldLength-fieldDecLength)){
                    ui->textEdit->setText(tr("数值的整数部分长度不能超过%1").arg(QString::number(fieldLength-fieldDecLength)));
                    return;
                }
                //小数部分
                QString intD=text.mid(index+1,-1);
                if(intD.length()>fieldDecLength){
                    ui->textEdit->setText(tr("数值的小数部分长度不能超过%1").arg(QString::number(fieldDecLength)));
                    return;
                }
                //如果没问题就好说
                modify=true;
                this->close();
            }
        }
    }
    //其他未知类型,仅校验长度
    else{
        if(textLength>fieldLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(fieldLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
}

void DialogModifyCell::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(fieldType=="N"&&arg1.isEmpty()){
        ui->textEdit->setText("数值型内容不建议填充空哟，确认需要填充空请继续保存，如果要使用全0填充，则填写一个0即可！");
    }
    else{
        ui->textEdit->setText(nullptr);
    }
}
