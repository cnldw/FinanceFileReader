/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "ofdfaultcause.h"

OFDFaultCause::OFDFaultCause()
{

}

const QString &OFDFaultCause::getName() const
{
    return name;
}

void OFDFaultCause::setName(const QString &newName)
{
    name = newName;
}

QString OFDFaultCause::getCause() const
{
    return cause;
}

void OFDFaultCause::setCause(const QString &value)
{
    cause = value;
}

QString OFDFaultCause::getConfig() const
{
    return config;
}

void OFDFaultCause::setConfig(const QString &value)
{
    config = value;
}
