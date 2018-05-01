#ifndef DIALOGABOUTAUTHOR_H
#define DIALOGABOUTAUTHOR_H

#include <QDialog>

namespace Ui {
class DialogAboutAuthor;
}

class DialogAboutAuthor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutAuthor(QWidget *parent = 0);
    ~DialogAboutAuthor();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogAboutAuthor *ui;
    int number=0;
};

#endif // DIALOGABOUTAUTHOR_H
