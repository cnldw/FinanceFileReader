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

#ifndef DBFFILECONFIG_H
#define DBFFILECONFIG_H

#include<QString>
#include<QList>
#include<QMap>

struct FileNameAndDescribe{
    QString fileName;
    QString fileDescribe;
};

class DbfFileConfig
{
public:
    DbfFileConfig();
    //使用的正则匹配文件名
    QString fileIniName;
    //适配的文件名信息
    QList<FileNameAndDescribe> supportFileList;
    QMap<QString,QString> fieldDescribe;

    QString getFileIniName() const;
    void setFileIniName(const QString &value);
    QList<FileNameAndDescribe> getSupportFileList() const;
    void setSupportFileList(const QList<FileNameAndDescribe> &value);
    QMap<QString, QString> getFieldDescribe() const;
    void setFieldDescribe(const QMap<QString, QString> &value);
};

#endif // DBFFILECONFIG_H
