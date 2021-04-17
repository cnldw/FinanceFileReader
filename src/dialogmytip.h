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
#ifndef DIALOGMYTIP_H
#define DIALOGMYTIP_H

#include <QDialog>
#include <QString>

namespace Ui {
class DialogMyTip;
}

/**
 * @brief The DialogMyTip class 自定义提示框
 */
class DialogMyTip : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMyTip(QString tip,QWidget *parent = nullptr,bool information=false);
    ~DialogMyTip();

    bool getBoolFlag() const;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogMyTip *ui;
    bool boolFlag=false;
};

#endif // DIALOGMYTIP_H
