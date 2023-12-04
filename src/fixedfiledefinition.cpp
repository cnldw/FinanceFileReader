/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "fixedfiledefinition.h"

bool FIXEDFileDefinition::getUseAble() const
{
    return useAble;
}

void FIXEDFileDefinition::setUseAble(bool value)
{
    useAble = value;
}

QString FIXEDFileDefinition::getMessage() const
{
    return message;
}

void FIXEDFileDefinition::setMessage(const QString &value)
{
    message = value;
}

QList<int> FIXEDFileDefinition::getFieldCountList() const
{
    return fieldCountList;
}

void FIXEDFileDefinition::setFieldCountList(const QList<int> &value)
{
    fieldCountList = value;
}

QList<FixedFieldDefinition> FIXEDFileDefinition::getFieldList() const
{
    return fieldList;
}

void FIXEDFileDefinition::setFieldList(const QList<FixedFieldDefinition> &value)
{
    fieldList = value;
}

QString FIXEDFileDefinition::getFileIni() const
{
    return fileIni;
}

void FIXEDFileDefinition::setFileIni(const QString &value)
{
    fileIni = value;
}

QString FIXEDFileDefinition::getFileName() const
{
    return fileName;
}

void FIXEDFileDefinition::setFileName(const QString &value)
{
    fileName = value;
}

QString FIXEDFileDefinition::getFileNameWithVersion() const
{
    return fileNameWithVersion;
}

void FIXEDFileDefinition::setFileNameWithVersion(const QString &value)
{
    fileNameWithVersion = value;
}

QString FIXEDFileDefinition::getFileDescribe() const
{
    return fileDescribe;
}

void FIXEDFileDefinition::setFileDescribe(const QString &value)
{
    fileDescribe = value;
}

QString FIXEDFileDefinition::getFieldlengthtype() const
{
    return fieldLengthType;
}

void FIXEDFileDefinition::setFieldlengthtype(const QString &value)
{
    fieldLengthType = value;
}

QString FIXEDFileDefinition::getEcoding() const
{
    return ecoding;
}

void FIXEDFileDefinition::setEcoding(const QString &value)
{
    ecoding = value;
}

int FIXEDFileDefinition::getDataRowBeginIndex() const
{
    return dataRowBeginIndex;
}

void FIXEDFileDefinition::setDataRowBeginIndex(int value)
{
    dataRowBeginIndex = value;
}

int FIXEDFileDefinition::getEndIgnoreRow() const
{
    return endIgnoreRow;
}

void FIXEDFileDefinition::setEndIgnoreRow(int value)
{
    endIgnoreRow = value;
}

QHash<int, int> FIXEDFileDefinition::getRowLengthHash() const
{
    return rowLengthHash;
}

void FIXEDFileDefinition::setRowLengthHash(const QHash<int, int> &value)
{
    rowLengthHash = value;
}

int FIXEDFileDefinition::getFieldCountMax() const
{
    return fieldCountMax;
}

void FIXEDFileDefinition::setFieldCountMax(int value)
{
    fieldCountMax = value;
}

const QString &FIXEDFileDefinition::getFirstrowcheck() const
{
    return firstrowcheck;
}

void FIXEDFileDefinition::setFirstrowcheck(const QString &newFirstrowcheck)
{
    firstrowcheck = newFirstrowcheck;
}

const QString &FIXEDFileDefinition::getLastrowcheck() const
{
    return lastrowcheck;
}

void FIXEDFileDefinition::setLastrowcheck(const QString &newLastrowcheck)
{
    lastrowcheck = newLastrowcheck;
}

int FIXEDFileDefinition::getVersioncheckrow() const
{
    return versioncheckrow;
}

void FIXEDFileDefinition::setVersioncheckrow(int newVersioncheckrow)
{
    versioncheckrow = newVersioncheckrow;
}

const QString &FIXEDFileDefinition::getVersion() const
{
    return version;
}

void FIXEDFileDefinition::setVersion(const QString &newVersion)
{
    version = newVersion;
}

int FIXEDFileDefinition::getFieldcountcheckrow() const
{
    return fieldcountcheckrow;
}

void FIXEDFileDefinition::setFieldcountcheckrow(int newFieldcountcheckrow)
{
    fieldcountcheckrow = newFieldcountcheckrow;
}

int FIXEDFileDefinition::getFielddetailcheckbeginrow() const
{
    return fielddetailcheckbeginrow;
}

void FIXEDFileDefinition::setFielddetailcheckbeginrow(int newFielddetailcheckbeginrow)
{
    fielddetailcheckbeginrow = newFielddetailcheckbeginrow;
}

int FIXEDFileDefinition::getRowcountcheckrow() const
{
    return rowcountcheckrow;
}

void FIXEDFileDefinition::setRowcountcheckrow(int newRowcountcheckrow)
{
    rowcountcheckrow = newRowcountcheckrow;
}

int FIXEDFileDefinition::getVersioncheckmode() const
{
    return versioncheckmode;
}

void FIXEDFileDefinition::setVersioncheckmode(int newVersioncheckmode)
{
    versioncheckmode = newVersioncheckmode;
}

QList<fieldcheckitem> FIXEDFileDefinition::getFieldcheckList() const
{
    return fieldcheckList;
}

void FIXEDFileDefinition::addFieldcheckItem(const fieldcheckitem &value)
{
    fieldcheckList.append(value);
}

void FIXEDFileDefinition::clearFieldcheckItemList(){
    fieldcheckList.clear();
}

void FIXEDFileDefinition::setFieldcheckItemList(const QList<fieldcheckitem> &value){
    fieldcheckList=value;
}

const QString &FIXEDFileDefinition::getConfigSegment() const
{
    return configSegment;
}

void FIXEDFileDefinition::setConfigSegment(const QString &newConfigSegment)
{
    configSegment = newConfigSegment;
}

const QList<uint> &FIXEDFileDefinition::getPrimaryKeyFieldList() const
{
    return primaryKeyFieldList;
}

void FIXEDFileDefinition::setPrimaryKeyFieldList(const QList<uint> &newPrimaryKeyFieldList)
{
    primaryKeyFieldList = newPrimaryKeyFieldList;
}

FIXEDFileDefinition::FIXEDFileDefinition()
{
    
}

FIXEDFileDefinition::~FIXEDFileDefinition()
{
    
}
