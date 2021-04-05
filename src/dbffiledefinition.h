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
#ifndef DBFFILEDEFINITION_H
#define DBFFILEDEFINITION_H
#include <QList>
#include <QString>
#include "src/dbffielddefinition.h"


class DbfFileDefinition
{
private:
    //字段信息
    QList <DbfFieldDefinition> fieldList;
    //所属配置文件
    QString fileIni;
    QString fileDescribe;
    QString fileType;
    int fieldCount;
    //编码信息
    QString ecoding;

public:
    DbfFileDefinition();
    ~DbfFileDefinition();
    QList<DbfFieldDefinition> getFieldList() const;
    void setFieldList(const QList<DbfFieldDefinition> &value);
    QString getFileIni() const;
    void setFileIni(const QString &value);
    QString getFileDescribe() const;
    void setFileDescribe(const QString &value);
    int getFieldCount() const;
    void setFieldCount(int value);
    QString getEcoding() const;
    void setEcoding(const QString &value);
    QString getFileType() const;
    void setFileType(const QString &value);
};

#endif // DBFFILEDEFINITION_H
