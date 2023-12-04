/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGCHOOSEFIELDCHECKEXPORTREPORT_H
#define DIALOGCHOOSEFIELDCHECKEXPORTREPORT_H

#include <QDialog>
#include <QMap>
#include <QDebug>
#include "src/utils.h"

namespace Ui {
class DialogChooseFieldCheckExportReport;
}

class DialogChooseFieldCheckExportReport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseFieldCheckExportReport(QWidget *parent = nullptr);
    ~DialogChooseFieldCheckExportReport();

private slots:

    void on_pushButton3_clicked();

signals:
    void sendFieldExportConfig(QMap<QString,int> config);

private:
    Ui::DialogChooseFieldCheckExportReport *ui;
};

#endif // DIALOGCHOOSEFIELDCHECKEXPORTREPORT_H
