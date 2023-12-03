/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "faultcause.h"

FaultCause::FaultCause()
{

}

int FaultCause::getConfigIndex() const
{
    return configIndex;
}

void FaultCause::setConfigIndex(int value)
{
    configIndex = value;
}

QString FaultCause::getCause() const
{
    return cause;
}

void FaultCause::setCause(const QString &value)
{
    cause = value;
}

const matchIndex &FaultCause::getConfigIndex2() const
{
    return configIndex2;
}

void FaultCause::setConfigIndex2(const matchIndex &newConfigIndex2)
{
    configIndex2 = newConfigIndex2;
}
