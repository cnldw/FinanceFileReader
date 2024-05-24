/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogpreference.h"
#include "ui_dialogpreference.h"
#include "src/utils.h"

DialogPreference::DialogPreference(QMap <QString,int> * par,QMap <QString,QString> * parstr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreference)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
    this->par=par;
    this->parstr=parstr;
#ifdef Q_OS_MAC
    ui->pushButton->setMinimumHeight(32);
    ui->pushButton_2->setMinimumHeight(32);
#endif
    //从主界面获取当前的参数值
    if(par->contains("compresslevel")){
        if(par->value("compresslevel")==0){
            ui->comboBox->setCurrentIndex(0);
        }
        else if(par->value("compresslevel")==-1){
            ui->comboBox->setCurrentIndex(1);
        }
        else if(par->value("compresslevel")==1){
            ui->comboBox->setCurrentIndex(2);
        }
        else if(par->value("compresslevel")==5){
            ui->comboBox->setCurrentIndex(3);
        }
        else if(par->value("compresslevel")==9){
            ui->comboBox->setCurrentIndex(4);
        }
    }
    if(par->contains("defaultviewmode")){
        if(par->value("defaultviewmode")==0){
            ui->comboBox_2->setCurrentIndex(0);
        }
        else if(par->value("defaultviewmode")==1){
            ui->comboBox_2->setCurrentIndex(1);
        }
    }
    if(par->contains("defaultnewfilemode")){
        if(par->value("defaultnewfilemode")==0){
            ui->comboBox_3->setCurrentIndex(0);
        }
        else if(par->value("defaultnewfilemode")==1){
            ui->comboBox_3->setCurrentIndex(1);
        }
        else if(par->value("defaultnewfilemode")==2){
            ui->comboBox_3->setCurrentIndex(2);
        }
    }
    if(par->contains("defaultpagesizetype")){
        if(par->value("defaultpagesizetype")==0){
            ui->comboBox_4->setCurrentIndex(0);
        }
        else if(par->value("defaultpagesizetype")==1){
            ui->comboBox_4->setCurrentIndex(1);
        }
        else if(par->value("defaultpagesizetype")==2){
            ui->comboBox_4->setCurrentIndex(2);
        }
        else if(par->value("defaultpagesizetype")==3){
            ui->comboBox_4->setCurrentIndex(3);
        }
        else if(par->value("defaultpagesizetype")==4){
            ui->comboBox_4->setCurrentIndex(4);
        }
        //找不到就按10w
        else{
            ui->comboBox_4->setCurrentIndex(0);
        }
    }
    if(par->contains("defaultnewfilemode")){
        if(par->value("defaultnewfilemode")==0){
            ui->comboBox_3->setCurrentIndex(0);
        }
        else if(par->value("defaultnewfilemode")==1){
            ui->comboBox_3->setCurrentIndex(1);
        }
        else if(par->value("defaultnewfilemode")==2){
            ui->comboBox_3->setCurrentIndex(2);
        }
    }
    if(par->contains("enablefieldcheck")){
        if(par->value("enablefieldcheck")==0){
            ui->checkBox->setChecked(false);
        }
        else if(par->value("enablefieldcheck")==1){
            ui->checkBox->setChecked(true);
        }
    }
    if(par->contains("standfontsize")){
        if(par->value("standfontsize")==0){
            ui->comboBox_5->setCurrentIndex(0);
        }
        else if(par->value("standfontsize")==1){
            ui->comboBox_5->setCurrentIndex(1);
        }
        else if(par->value("standfontsize")==2){
            ui->comboBox_5->setCurrentIndex(2);
        }
    }
    if(parstr->contains("firstuifont")&&!parstr->value("firstuifont").isEmpty()){
        QFontDatabase fontDatabase;
        if(fontDatabase.families().count()>0){
            for(int i=0;i<fontDatabase.families().count();i++){
                if(fontDatabase.families().at(i)==parstr->value("firstuifont")){
                    ui->fontComboBox_2->setCurrentFont(QFont(parstr->value("firstuifont")));
                    break;
                }
            }
        }
    }
    if(parstr->contains("firstrarecharactersfont")&&!parstr->value("firstrarecharactersfont").isEmpty()){
        QFontDatabase fontDatabase;
        if(fontDatabase.families().count()>0){
            for(int i=0;i<fontDatabase.families().count();i++){
                if(fontDatabase.families().at(i)==parstr->value("firstrarecharactersfont")){
                    ui->fontComboBox->setCurrentFont(QFont(parstr->value("firstrarecharactersfont")));
                    break;
                }
            }
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
        par->insert("compresslevel",0);

    }
    else if(index==1){
        par->insert("compresslevel",-1);

    }
    else if(index==2){
        par->insert("compresslevel",1);

    }
    else if(index==3){
        par->insert("compresslevel",5);
    }
    else if(index==4){
        par->insert("compresslevel",9);
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

bool DialogPreference::getSaveFlag() const
{
    return saveFlag;
}

void DialogPreference::on_comboBox_2_currentIndexChanged(int index)
{
    if(index==0){
        par->insert("defaultviewmode",0);
    }
    else if(index==1){
        par->insert("defaultviewmode",1);

    }
}

void DialogPreference::on_comboBox_3_currentIndexChanged(int index)
{
    if(index==0){
        par->insert("defaultnewfilemode",0);
    }
    else if(index==1){
        par->insert("defaultnewfilemode",1);
    }
    else if(index==2){
        par->insert("defaultnewfilemode",2);

    }
}

void DialogPreference::on_comboBox_4_currentIndexChanged(int index)
{
    if(index==0){
        par->insert("defaultpagesizetype",0);
    }
    else if(index==1){
        par->insert("defaultpagesizetype",1);
    }
    else if(index==2){
        par->insert("defaultpagesizetype",2);
    }
    else if(index==3){
        par->insert("defaultpagesizetype",3);
    }
    else if(index==4){
        par->insert("defaultpagesizetype",4);
    }
}

void DialogPreference::on_checkBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1)
    par->insert("enablefieldcheck",ui->checkBox->isChecked()?1:0);
}

void DialogPreference::on_fontComboBox_currentFontChanged(const QFont &f)
{
    parstr->insert("firstrarecharactersfont",f.family());
}


void DialogPreference::on_fontComboBox_2_currentFontChanged(const QFont &f)
{
    parstr->insert("firstuifont",f.family());
}

void DialogPreference::on_comboBox_5_currentIndexChanged(int index)
{
    par->insert("standfontsize",index);
}

