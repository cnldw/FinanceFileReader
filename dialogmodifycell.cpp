#include "dialogmodifycell.h"
#include "ui_dialogmodifycell.h"

DialogModifyCell::DialogModifyCell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyCell)
{
    ui->setupUi(this);
}

DialogModifyCell::~DialogModifyCell()
{
    delete ui;
}
