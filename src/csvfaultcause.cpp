#include "csvfaultcause.h"

CsvFaultCause::CsvFaultCause()
{

}

int CsvFaultCause::getConfigIndex() const
{
    return configIndex;
}

void CsvFaultCause::setConfigIndex(int value)
{
    configIndex = value;
}

QString CsvFaultCause::getCause() const
{
    return cause;
}

void CsvFaultCause::setCause(const QString &value)
{
    cause = value;
}
