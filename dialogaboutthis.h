#ifndef DIALOGABOUTTHIS_H
#define DIALOGABOUTTHIS_H

#include <QDialog>
#include "utils.h"
namespace Ui {
class DialogAboutThis;
}

class DialogAboutThis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutThis(QWidget *parent = 0);
    ~DialogAboutThis();

private:
    Ui::DialogAboutThis *ui;
};

#endif // DIALOGABOUTTHIS_H
