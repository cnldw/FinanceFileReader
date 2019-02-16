#include "dialogpreference.h"
#include "ui_dialogpreference.h"

DialogPreference::DialogPreference(QMap <QString,QString> par,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreference)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    QList<QString> keys=par.keys();
    //从主界面获取当前的参数值
    if(keys.contains("compresslevel")){
        if(par.value("compresslevel")=="0"){
            this->dataCompressLevel=0;
            ui->comboBox->setCurrentIndex(0);
        }
        else if(par.value("compresslevel")=="-1"){
            this->dataCompressLevel=-1;
            ui->comboBox->setCurrentIndex(1);
        }
        else if(par.value("compresslevel")=="1"){
            this->dataCompressLevel=1;
            ui->comboBox->setCurrentIndex(2);
        }
        else if(par.value("compresslevel")=="5"){
            this->dataCompressLevel=5;
            ui->comboBox->setCurrentIndex(3);
        }
        else if(par.value("compresslevel")=="9"){
            this->dataCompressLevel=9;
            ui->comboBox->setCurrentIndex(4);
        }
    }
    if(keys.contains("defaultviewmode")){
        if(par.value("defaultviewmode")=="0"){
            this->defaultViewMode="0";
            ui->comboBox_2->setCurrentIndex(0);
        }
        else if(par.value("defaultviewmode")=="1"){
            this->defaultViewMode="1";
            ui->comboBox_2->setCurrentIndex(1);
        }
    }
    if(keys.contains("defaultnewfilemode")){
        if(par.value("defaultnewfilemode")=="0"){
            this->defaultNewFileMode="0";
            ui->comboBox_3->setCurrentIndex(0);
        }
        else if(par.value("defaultnewfilemode")=="1"){
            this->defaultNewFileMode="1";
            ui->comboBox_3->setCurrentIndex(1);
        }
        else if(par.value("defaultnewfilemode")=="2"){
            this->defaultNewFileMode="2";
            ui->comboBox_3->setCurrentIndex(2);
        }
    }
}

DialogPreference::~DialogPreference()
{
    delete ui;
}

void DialogPreference::on_comboBox_currentIndexChanged(int index)
{
    if(index==0){
        dataCompressLevel=0;
    }
    else if(index==1){
        dataCompressLevel=-1;
    }
    else if(index==2){
        dataCompressLevel=1;
    }
    else if(index==3){
        dataCompressLevel=5;
    }
    else if(index==4){
        dataCompressLevel=9;
    }
}

void DialogPreference::on_pushButton_clicked()
{
    this->saveFlag=true;
    this->close();
}

void DialogPreference::on_pushButton_2_clicked()
{
    this->saveFlag=false;
    this->close();
}

QString DialogPreference::getDefaultViewMode() const
{
    return defaultViewMode;
}

int DialogPreference::getDataCompressLevel() const
{
    return dataCompressLevel;
}

bool DialogPreference::getSaveFlag() const
{
    return saveFlag;
}

void DialogPreference::on_comboBox_2_currentIndexChanged(int index)
{
    if(index==0){
        defaultViewMode="0";
    }
    else if(index==1){
        defaultViewMode="1";
    }
}

QString DialogPreference::getDefaultNewFileMode() const
{
    return defaultNewFileMode;
}

void DialogPreference::on_comboBox_3_currentIndexChanged(int index)
{
    if(index==0){
        defaultNewFileMode="0";
    }
    else if(index==1){
        defaultNewFileMode="1";
    }
    else if(index==2){
        defaultNewFileMode="2";
    }
}
