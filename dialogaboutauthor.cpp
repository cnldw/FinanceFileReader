#include "dialogaboutauthor.h"
#include "ui_dialogaboutauthor.h"
#include <QApplication>
#include "gui/qgameboard.h"

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

void DialogAboutAuthor::on_pushButton_clicked()
{
        number++;
        if(number>=8){
            QGameBoard *board=new QGameBoard();
            board->setWindowTitle("2048-欢迎来到2048彩蛋");
            board->show();
            this->close();
        }
}
