/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef OFDCODEINFO_H
#define OFDCODEINFO_H

#include <QString>

class OFDCodeInfo
{
public:
    OFDCodeInfo();
    QString getCode()const;
    QString getVersion()const;
    QString getName()const;
    void setCode(QString name);
    void setName(QString name);

private:
    QString code;
    QString name;
};

#endif // OFDCODEINFO_H
