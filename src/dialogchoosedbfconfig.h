/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGCHOOSEDBFCONFIG_H
#define DIALOGCHOOSEDBFCONFIG_H

#include <QDialog>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class DialogChooseDBFConfig;
}

class DialogChooseDBFConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseDBFConfig(QList<QStringList> * matchDBF,QWidget *parent = nullptr);
    ~DialogChooseDBFConfig();
    int getChooeseIndex() const;

    bool getConfirm() const;
    void setConfirm(bool value);

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_itemSelectionChanged();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::DialogChooseDBFConfig *ui;

    int chooeseIndex=0;
    bool  confirm=false;
    QTableWidget * ptr_table;
};

#endif // DIALOGCHOOSEDBFCONFIG_H
