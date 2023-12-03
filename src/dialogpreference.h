/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
