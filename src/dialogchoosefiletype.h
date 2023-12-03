/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGCHOOSEFILETYPE_H
#define DIALOGCHOOSEFILETYPE_H

#include <QDialog>

namespace Ui {
class DialogChooseFileType;
}

/**
 * @brief The DialogChooseFileType class 选择文件类别
 */
class DialogChooseFileType : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseFileType(QWidget *parent = nullptr);
    ~DialogChooseFileType();

    int getFileType() const;
    void setFileType(int value);

private slots:
    void on_pushButtonCsv_clicked();

    void on_pushButtonFixed_clicked();

private:
    Ui::DialogChooseFileType *ui;
    int fileType=0;
};

#endif // DIALOGCHOOSEFILETYPE_H
