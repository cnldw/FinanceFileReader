/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
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
