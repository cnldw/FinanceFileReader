#include "dialogeditheaderfooter.h"
#include "ui_dialogeditheaderfooter.h"

DialogEditHeaderFooter::DialogEditHeaderFooter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditHeaderFooter)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);
}

DialogEditHeaderFooter::~DialogEditHeaderFooter()
{
    delete ui;
}
