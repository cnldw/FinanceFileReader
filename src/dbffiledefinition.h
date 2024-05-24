/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
