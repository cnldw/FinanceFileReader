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
#ifndef DIALOGPREFERENCE_H
#define DIALOGPREFERENCE_H

#include <QDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QString>
#include <QMap>
namespace Ui {
class DialogPreference;
}

/**
 * @brief The DialogPreference class 设置界面
 */
class DialogPreference : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPreference(QMap<QString,int> * par,QWidget *parent = nullptr);
    ~DialogPreference();
    bool getSaveFlag() const;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::DialogPreference *ui;
    QMap <QString,int> *par;
    bool saveFlag=false;
};

#endif // DIALOGPREFERENCE_H
