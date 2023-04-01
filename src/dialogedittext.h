#ifndef DIALOGEDITTEXT_H
#define DIALOGEDITTEXT_H

#include <QDialog>
#include <QDebug>


namespace Ui {
class DialogEditText;
}

class DialogEditText : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditText(QString title,QStringList fieldList,int importType,QWidget *parent = nullptr);
    ~DialogEditText();

private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogEditText *ui;
    QStringList fieldList;
    int importType;
    QString text;
signals:
    void sendImportString(QString data,int type,bool clearOldData);
};

#endif // DIALOGEDITTEXT_H
