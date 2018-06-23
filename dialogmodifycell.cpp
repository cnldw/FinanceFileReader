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
    ui->lineEdit_4->setFocus();
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
        ui->lineEdit->setText("TEXT(文本型)");
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    //内容
    QString text=ui->lineEdit_4->text();
    //长度
    int textLength=text.toLocal8Bit().length();
    //字符类和文本类的只需要判断长度
    if(filedType=="C"||filedType=="TEXT"){
        //长度校验
        if(textLength>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
        }
        else{
            modify=true;
            this->close();
        }
    }
    //数字字符型
    else if(filedType=="A"){
        //长度校验
        if(textLength>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
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
    else if(filedType=="N"){
        //空数据,空数据由主窗口设置为全0
        if(text.isEmpty()){
            modify=true;
            this->close();
        }
        bool ok=false;
        text.toFloat(&ok);
        //如果无法转化为float直接认为不合规
        if(!ok){
            ui->textEdit->setText(tr("输入的内容不是一个有效的数值"));
        }
        else{
            //无小数部分
            if(!text.contains("."))
            {
                if(textLength>(filedLength-filedDecLength)){
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
                int index=text.indexOf(".");
                //整数部分
                QString intS=text.mid(0,index);
                if(intS.length()>(filedLength-filedDecLength)){
                    ui->textEdit->setText(tr("数值的整数部分长度不能超过%1").arg(QString::number(filedLength-filedDecLength)));
                    return;
                }
                //小数部分
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
    //其他未知类型,仅校验长度
    else{
        if(textLength>filedLength){
            ui->textEdit->setText(tr("长度不能超过%1,请检查").arg(filedLength));
            return;
        }
        else{
            modify=true;
            this->close();
        }
    }
}
