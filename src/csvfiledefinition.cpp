/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
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

QString CsvFileDefinition::getFileIni() const
{
    return fileIni;
}

void CsvFileDefinition::setFileIni(const QString &value)
{
    fileIni = value;
}

QString CsvFileDefinition::getEndwithflag() const
{
    return endwithflag;
}

void CsvFileDefinition::setEndwithflag(const QString &value)
{
    endwithflag = value;
}

QString CsvFileDefinition::getFileDescribe() const
{
    return fileDescribe;
}

void CsvFileDefinition::setFileDescribe(const QString &value)
{
    fileDescribe = value;
}

QString CsvFileDefinition::getFileNameWithCount() const
{
    return fileNameWithCount;
}

void CsvFileDefinition::setFileNameWithCount(const QString &value)
{
    fileNameWithCount = value;
}

int CsvFileDefinition::getEndIgnoreRow() const
{
    return endIgnoreRow;
}

void CsvFileDefinition::setEndIgnoreRow(int value)
{
    endIgnoreRow = value;
}

bool CsvFileDefinition::getClearQuotes() const
{
    return clearQuotes;
}

void CsvFileDefinition::setClearQuotes(bool value)
{
    clearQuotes = value;
}

bool CsvFileDefinition::getFfAuto() const
{
    return ffAuto;
}

void CsvFileDefinition::setFfAuto(bool value)
{
    ffAuto = value;
}

bool CsvFileDefinition::getTrim() const
{
    return trim;
}

void CsvFileDefinition::setTrim(bool value)
{
    trim = value;
}

int CsvFileDefinition::getVersioncheckrow() const
{
    return versioncheckrow;
}

void CsvFileDefinition::setVersioncheckrow(int newVersioncheckrow)
{
    versioncheckrow = newVersioncheckrow;
}

int CsvFileDefinition::getVersioncheckmode() const
{
    return versioncheckmode;
}

void CsvFileDefinition::setVersioncheckmode(int newVersioncheckmode)
{
    versioncheckmode = newVersioncheckmode;
}

const QString &CsvFileDefinition::getVersion() const
{
    return version;
}

void CsvFileDefinition::setVersion(const QString &newVersion)
{
    version = newVersion;
}

const QString &CsvFileDefinition::getFirstrowcheck() const
{
    return firstrowcheck;
}

void CsvFileDefinition::setFirstrowcheck(const QString &newFirstrowcheck)
{
    firstrowcheck = newFirstrowcheck;
}

const QString &CsvFileDefinition::getLastrowcheck() const
{
    return lastrowcheck;
}

void CsvFileDefinition::setLastrowcheck(const QString &newLastrowcheck)
{
    lastrowcheck = newLastrowcheck;
}

QList<fieldcheckitem> CsvFileDefinition::getFieldcheckList() const
{
    return fieldcheckList;
}

void CsvFileDefinition::addFieldcheckItem(const fieldcheckitem &value)
{
    fieldcheckList.append(value);
}

void CsvFileDefinition::clearFieldcheckItemList(){
    fieldcheckList.clear();
}

void CsvFileDefinition::setFieldcheckItemList(const QList<fieldcheckitem> &value){
    fieldcheckList=value;
}

const QString &CsvFileDefinition::getConfigSegment() const
{
    return configSegment;
}

void CsvFileDefinition::setConfigSegment(const QString &newConfigSegment)
{
    configSegment = newConfigSegment;
}

bool CsvFileDefinition::getAutoecoding() const
{
    return autoecoding;
}

void CsvFileDefinition::setAutoecoding(bool newAutoecoding)
{
    autoecoding = newAutoecoding;
}

const QList<uint> &CsvFileDefinition::getPrimaryKeyFieldList() const
{
    return primaryKeyFieldList;
}

void CsvFileDefinition::setPrimaryKeyFieldList(const QList<uint> &newPrimaryKeyFieldList)
{
    primaryKeyFieldList = newPrimaryKeyFieldList;
}

CsvFileDefinition::CsvFileDefinition()
{
    
}
