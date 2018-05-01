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

private:
    Ui::DialogAboutAuthor *ui;
};

#endif // DIALOGABOUTAUTHOR_H
