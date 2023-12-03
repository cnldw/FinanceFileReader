/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGCHOOSEOFDCONFIG_H
#define DIALOGCHOOSEOFDCONFIG_H

#include <QDialog>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "src/ofdfiledefinition.h"

namespace Ui {
class DialogChooseOFDConfig;
}

class DialogChooseOFDConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseOFDConfig(QString ofdini,QList<OFDFileDefinition> * matchOFD,QWidget *parent = nullptr);
    ~DialogChooseOFDConfig();

    int getChooeseIndex() const;

    bool getConfirm() const;
    void setConfirm(bool value);

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DialogChooseOFDConfig *ui;
    //默认选择第一个
    int chooeseIndex=0;
    bool  confirm=false;
    QTableWidget * ptr_table;
};

#endif // DIALOGCHOOSEOFDCONFIG_H
