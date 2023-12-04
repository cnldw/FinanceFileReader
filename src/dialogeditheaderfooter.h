/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGEDITHEADERFOOTER_H
#define DIALOGEDITHEADERFOOTER_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QClipboard>
#include <QPoint>
#include <QMenu>
#include <QDebug>
#include "src/ofdfiledefinition.h"
#include "src/publicdefine.h"
#include "src/utils.h"

namespace Ui {
class DialogEditHeaderFooter;
}

class DialogEditHeaderFooter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditHeaderFooter(OFDFileDefinition *ofd,QList<QStringList> header,QList<QStringList>  footer,QWidget *parent = nullptr);
    ~DialogEditHeaderFooter();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    void sendNewHeaderAndFooter(QStringList header,QStringList footer);

private:
    Ui::DialogEditHeaderFooter *ui;
    QTableWidget * ptr_header_table;
    QTableWidget * ptr_footer_table;
};

#endif // DIALOGEDITHEADERFOOTER_H
