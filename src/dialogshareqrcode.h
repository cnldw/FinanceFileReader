#ifndef DIALOGSHAREQRCODE_H
#define DIALOGSHAREQRCODE_H

#include <QDialog>
#include <QString>
#include "src/qrcode/QrCode.hpp"

namespace Ui {
class DialogShareQrCode;
}

class DialogShareQrCode : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShareQrCode(QString text,QWidget *parent = nullptr);
    ~DialogShareQrCode();

private:
    Ui::DialogShareQrCode *ui;
    void updateQrCode( QString text );
};

#endif // DIALOGSHAREQRCODE_H
