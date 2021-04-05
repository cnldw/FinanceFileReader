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
#include "dialogshareqrcode.h"
#include "ui_dialogshareqrcode.h"

DialogShareQrCode::DialogShareQrCode(QString text,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShareQrCode)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    updateQrCode(text);
}

DialogShareQrCode::~DialogShareQrCode()
{
    delete ui;
}


void DialogShareQrCode::updateQrCode( QString text )
{ using namespace qrcodegen;
    QrCode qr = QrCode::encodeText( text.toUtf8().data(), QrCode::Ecc::MEDIUM );
    qint32 sz = qr.getSize();
    QImage im(sz,sz, QImage::Format_RGB32);
    QRgb black = qRgb(  0,  0,  0);
    QRgb white = qRgb(255,255,255);
    for (int y = 0; y < sz; y++)
        for (int x = 0; x < sz; x++)
            im.setPixel(x,y,qr.getModule(x, y) ? black : white );
    ui->label->setPixmap( QPixmap::fromImage(im.scaled(400,400,Qt::KeepAspectRatio,Qt::FastTransformation),Qt::MonoOnly) );
}
