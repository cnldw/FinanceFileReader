#ifndef FORMWEBTOOLS_H
#define FORMWEBTOOLS_H

#include <QWidget>
#include "src/miniblink/miniblink.h"
#include "QFileInfo"


namespace Ui {
class FormWebTools;
}

class FormWebTools : public QWidget
{
    Q_OBJECT

public:
    explicit FormWebTools(QString title,QString path,QWidget *parent = nullptr);
    ~FormWebTools();

private:
    Ui::FormWebTools *ui;
    miniblink *webView;

};

#endif // FORMWEBTOOLS_H
