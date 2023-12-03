/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dbffiledefinition.h"

QList<DbfFieldDefinition> DbfFileDefinition::getFieldList() const
{
    return fieldList;
}

void DbfFileDefinition::setFieldList(const QList<DbfFieldDefinition> &value)
{
    fieldList = value;
}

QString DbfFileDefinition::getFileIni() const
{
    return fileIni;
}

void DbfFileDefinition::setFileIni(const QString &value)
{
    fileIni = value;
}

QString DbfFileDefinition::getFileDescribe() const
{
    return fileDescribe;
}

void DbfFileDefinition::setFileDescribe(const QString &value)
{
    fileDescribe = value;
}

int DbfFileDefinition::getFieldCount() const
{
    return fieldCount;
}

void DbfFileDefinition::setFieldCount(int value)
{
    fieldCount = value;
}

QString DbfFileDefinition::getEcoding() const
{
    return ecoding;
}

void DbfFileDefinition::setEcoding(const QString &value)
{
    ecoding = value;
}

QString DbfFileDefinition::getFileType() const
{
    return fileType;
}

void DbfFileDefinition::setFileType(const QString &value)
{
    fileType = value;
}

DbfFileDefinition::DbfFileDefinition()
{
    
}

DbfFileDefinition::~DbfFileDefinition()
{
    
}
