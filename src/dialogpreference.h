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
    explicit DialogPreference(QMap<QString,QString> par,QWidget *parent = 0);
    ~DialogPreference();

    bool getSaveFlag() const;

    int getDataCompressLevel() const;

    QString getDefaultViewMode() const;

    QString getDefaultNewFileMode() const;

    QString getDefaultPageSizeType() const;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

private:
    Ui::DialogPreference *ui;
    bool saveFlag=false;
    int dataCompressLevel=-1;
    QString defaultViewMode=0;
    QString defaultNewFileMode=0;
    QString defaultPageSizeType="0";
};

#endif // DIALOGPREFERENCE_H
