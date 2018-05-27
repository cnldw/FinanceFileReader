#ifndef DIALOGMERGETIP_H
#define DIALOGMERGETIP_H

#include <QDialog>

namespace Ui {
class DialogMergeTip;
}

class DialogMergeTip : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMergeTip(QWidget *parent = 0);
    bool getMergeFlag();
    ~DialogMergeTip();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogMergeTip *ui;
    bool flag=false;
};

#endif // DIALOGMERGETIP_H
