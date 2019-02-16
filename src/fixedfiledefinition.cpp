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

FIXEDFileDefinition::FIXEDFileDefinition()
{
    
}

FIXEDFileDefinition::~FIXEDFileDefinition()
{
    
}
