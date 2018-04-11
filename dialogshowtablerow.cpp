#include "dialogshowtablerow.h"
#include "ui_dialogshowtablerow.h"

DialogShowTableRow::DialogShowTableRow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowTableRow)
{
    ui->setupUi(this);
}

DialogShowTableRow::~DialogShowTableRow()
{
    delete ui;
}
