#ifndef DIALOGEDITHEADERFOOTER_H
#define DIALOGEDITHEADERFOOTER_H

#include <QDialog>

namespace Ui {
class DialogEditHeaderFooter;
}

class DialogEditHeaderFooter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditHeaderFooter(QWidget *parent = nullptr);
    ~DialogEditHeaderFooter();

private slots:


private:
    Ui::DialogEditHeaderFooter *ui;
};

#endif // DIALOGEDITHEADERFOOTER_H
