#include "csvfiledefinition.h"

bool CsvFileDefinition::getUseAble() const
{
    return useAble;
}

void CsvFileDefinition::setUseAble(bool value)
{
    useAble = value;
}

QString CsvFileDefinition::getMessage() const
{
    return message;
}

void CsvFileDefinition::setMessage(const QString &value)
{
    message = value;
}

int CsvFileDefinition::getFieldCount() const
{
    return fieldCount;
}

void CsvFileDefinition::setFieldCount(int value)
{
    fieldCount = value;
}

QList<CsvFieldDefinition> CsvFileDefinition::getFieldList() const
{
    return fieldList;
}

void CsvFileDefinition::setFieldList(const QList<CsvFieldDefinition> &value)
{
    fieldList = value;
}

QString CsvFileDefinition::getFileName() const
{
    return fileName;
}

void CsvFileDefinition::setFileName(const QString &value)
{
    fileName = value;
}

QString CsvFileDefinition::getVersion() const
{
    return version;
}

void CsvFileDefinition::setVersion(const QString &value)
{
    version = value;
}

QString CsvFileDefinition::getSplit() const
{
    return split;
}

void CsvFileDefinition::setSplit(const QString &value)
{
    split = value;
}

int CsvFileDefinition::getDatabeginrowindex() const
{
    return databeginrowindex;
}

void CsvFileDefinition::setDatabeginrowindex(int value)
{
    databeginrowindex = value;
}

QString CsvFileDefinition::getEcoding() const
{
    return ecoding;
}

void CsvFileDefinition::setEcoding(const QString &value)
{
    ecoding = value;
}

int CsvFileDefinition::getTitlerowindex() const
{
    return titlerowindex;
}

void CsvFileDefinition::setTitlerowindex(int value)
{
    titlerowindex = value;
}

CsvFileDefinition::CsvFileDefinition()
{

}
