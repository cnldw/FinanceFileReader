/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#include <QApplication>
#include "src/dialogaboutauthor.h"
#include "ui_dialogaboutauthor.h"
#include "src/2048/gui/qgameboard.h"

DialogAboutAuthor::DialogAboutAuthor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutAuthor)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->textEdit->setText(QByteArray::fromBase64(AUTHOR_INFO));
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
