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
    explicit DialogMyTip(QString tip,QWidget *parent = nullptr);
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
