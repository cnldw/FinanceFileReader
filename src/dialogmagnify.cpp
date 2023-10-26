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

#include "dialogmagnify.h"
#include "ui_dialogmagnify.h"
#include "src/publicdefine.h"

DialogMagnify::DialogMagnify(QString text,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMagnify)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsMacOS));
#endif
#ifdef Q_OS_LINUX
    this->setStyleSheet(QString(FONTSIZE13).append(UIFontsLinux));
#endif
#ifdef Q_OS_WIN32
    this->setStyleSheet(UIFontsWindows);
#endif
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint);
    QString html;
    html.append("<div style=\"");
    html.append(FONTSIZE64);
    html.append("\"");
#ifdef Q_OS_MAC
    html.append(UIFontsMacOS);
#endif
#ifdef Q_OS_LINUX
    html.append(UIFontsLinux);
#endif
#ifdef Q_OS_WIN32
    html.append(UIFontsWindows);
#endif
    html.append("\">");
    html.append(text);
    html.append("</div>");
    ui->textEdit->setHtml(html);
}

DialogMagnify::~DialogMagnify()
{
    delete ui;
}
