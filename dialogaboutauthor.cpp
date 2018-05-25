#include <QApplication>
#include "dialogaboutauthor.h"
#include "ui_dialogaboutauthor.h"
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
        //2048游戏彩蛋,当点击作者头像超过4次时触发打开此游戏
        number++;
        if(number>=4){
            QGameBoard *board=new QGameBoard();
            board->setWindowTitle("2048-恭喜你发现了2048彩蛋,来玩一局吧~");
            board->show();
            this->close();
        }
}
