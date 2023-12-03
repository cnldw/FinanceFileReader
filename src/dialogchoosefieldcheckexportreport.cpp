/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogchoosefieldcheckexportreport.h"
#include "ui_dialogchoosefieldcheckexportreport.h"
#include "src/publicdefine.h"

DialogChooseFieldCheckExportReport::DialogChooseFieldCheckExportReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseFieldCheckExportReport)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
#ifdef Q_OS_MAC
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
#endif
#ifdef Q_OS_WIN32
    this->setStyleSheet(UIFontsWindows);
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

