#include "dialogmytip.h"
#include "ui_dialogmytip.h"

DialogMyTip::DialogMyTip(QString tip,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMyTip)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    ui->label->setText(tip);
    ui->label->setWordWrap(true);
}

DialogMyTip::~DialogMyTip()
{
    delete ui;
}

bool DialogMyTip::getBoolFlag() const
{
    return boolFlag;
}

void DialogMyTip::on_pushButton_clicked()
{
    this->boolFlag=true;
    this->close();
}

void DialogMyTip::on_pushButton_2_clicked()
{
    this->boolFlag=false;
    this->close();
}
