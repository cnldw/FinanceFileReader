/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef OFDFAULTCAUSE_H
#define OFDFAULTCAUSE_H

#include <QString>

class OFDFaultCause
{
public:
    OFDFaultCause();
    QString config;
    QString name;
    QString cause;
    QString getConfig() const;
    void setConfig(const QString &value);

    QString getCause() const;
    void setCause(const QString &value);

    const QString &getName() const;
    void setName(const QString &newName);

private:

};

#endif // OFDFAULTCAUSE_H
