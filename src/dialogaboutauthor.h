/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DIALOGABOUTAUTHOR_H
#define DIALOGABOUTAUTHOR_H

#include <QDialog>
#include <QByteArray>

namespace Ui {
class DialogAboutAuthor;
}

/**
 * @brief The DialogAboutAuthor class 关于作者
 */
class DialogAboutAuthor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutAuthor(QWidget *parent = nullptr);
    ~DialogAboutAuthor();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogAboutAuthor *ui;
    int number=0;
};

#endif // DIALOGABOUTAUTHOR_H
