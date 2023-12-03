/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
