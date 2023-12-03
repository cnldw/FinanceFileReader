/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef DBFFIELDDEFINITION_H
#define DBFFIELDDEFINITION_H

#include <QString>
#include "qdbffield.h"

class DbfFieldDefinition
{
public:
    DbfFieldDefinition();

    int getFieldType() const;
    void setFieldType(const int &value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFieldName() const;
    void setFieldName(const QString &value);

    QString getFieldDescribe() const;
    void setFieldDescribe(const QString &value);

    QString getFieldOriginName() const;
    void setFieldOriginName(const QString &value);

private:
    /*
     * Undefined -1
     * Character 0 字符型--数据长度不够的话右补空格,右trim
     * Date 1 日期型-直接tostring
     * FloatingPoint 2 浮点型
     * Logical 3 逻辑型-tostring
     * Memo 4 备注型
     * Number 5 -数值型
     * Integer 6 整数型
     * DateTime 7 日期时间型
     * Currency 8 货币型
     */
    int fieldType;
    int length;
    int decLength;
    //英文字段名
    QString fieldName;
    //字段名，不转大写,dbf文件里原生的
    QString fieldOriginName;
    //字段中文名
    QString fieldDescribe;

};

#endif // DBFFIELDDEFINITION_H
