#include "dialogaboutauthor.h"
#include "ui_dialogaboutauthor.h"
#include <QApplication>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "core/board.h"
#include "gui/qgameboard.h"
#include "gui/qgameoverwindow.h"

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
            number=0;
            srand(time(NULL));
            QGameBoard board;
            board.show();
            this->close();
        }
}
