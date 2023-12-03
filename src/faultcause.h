/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef CSVFAULTCAUSE_H
#define CSVFAULTCAUSE_H
#include <QString>
#include "src/configfile.h"

class FaultCause
{
public:
    FaultCause();

    int getConfigIndex() const;
    void setConfigIndex(int value);

    QString getCause() const;
    void setCause(const QString &value);

    const matchIndex &getConfigIndex2() const;
    void setConfigIndex2(const matchIndex &newConfigIndex2);

    const QString &getName() const;
    void setName(const QString &newName);

private:
    int configIndex;
    matchIndex configIndex2;
    QString cause;
};

#endif // CSVFAULTCAUSE_H
