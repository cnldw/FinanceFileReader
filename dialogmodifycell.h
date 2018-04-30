#ifndef DIALOGMODIFYCELL_H
#define DIALOGMODIFYCELL_H

#include <QDialog>
#include <QTextCodec>
namespace Ui {
class DialogModifyCell;
}

class DialogModifyCell : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyCell(QString filedType,int filedLength,int filedDecLength,QString value,QWidget *parent);
    ~DialogModifyCell();
    bool getModifyFlag();
    QString getValueNew();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogModifyCell *ui;
    bool modify=false;
    QString filedType="";
    int filedLength=0;
    int filedDecLength=0;
};

#endif // DIALOGMODIFYCELL_H
