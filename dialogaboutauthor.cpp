#include "dialogaboutauthor.h"
#include "ui_dialogaboutauthor.h"

DialogAboutAuthor::DialogAboutAuthor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutAuthor)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

DialogAboutAuthor::~DialogAboutAuthor()
{
    delete ui;
}
