/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dbffileconfig.h"

DbfFileConfig::DbfFileConfig()
{

}

QMap<QString, QString> DbfFileConfig::getFieldDescribe() const
{
    return fieldDescribe;
}

void DbfFileConfig::setFieldDescribe(const QMap<QString, QString> &value)
{
    fieldDescribe = value;
}

QList<FileNameAndDescribe> DbfFileConfig::getSupportFileList() const
{
    return supportFileList;
}

void DbfFileConfig::setSupportFileList(const QList<FileNameAndDescribe> &value)
{
    supportFileList = value;
}

QString DbfFileConfig::getFileIniName() const
{
    return fileIniName;
}

void DbfFileConfig::setFileIniName(const QString &value)
{
    fileIniName = value;
}
