/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dialogchoosefiletype.h"
#include "ui_dialogchoosefiletype.h"
#include "src/utils.h"

DialogChooseFileType::DialogChooseFileType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseFileType)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    Utils::setDefaultWindowFonts(this);
}

DialogChooseFileType::~DialogChooseFileType()
{
    delete ui;
}

void DialogChooseFileType::on_pushButtonCsv_clicked()
{
    this->setFileType(2);
    this->close();
}

void DialogChooseFileType::on_pushButtonFixed_clicked()
{
    this->setFileType(3);
    this->close();
}

int DialogChooseFileType::getFileType() const
{
    return fileType;
}

void DialogChooseFileType::setFileType(int value)
{
    fileType = value;
}
