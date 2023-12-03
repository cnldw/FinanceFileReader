/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef FIXEDFIELDDEFINITION_H
#define FIXEDFIELDDEFINITION_H

#include <QString>

class FixedFieldDefinition
{
public:
    FixedFieldDefinition();

    QString getFieldType() const;
    void setFieldType(const QString &value);

    int getRowBeginIndex() const;
    void setRowBeginIndex(int value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFieldName() const;
    void setFieldName(const QString &value);

    QString getFieldDescribe() const;
    void setFieldDescribe(const QString &value);

private:
    QString fieldType;
    int rowBeginIndex;
    int length;
    int decLength;
    //这个字段应该填充英文字段名，当前不强制要求
    QString fieldName;
    //字段中文名
    QString fieldDescribe;

};

#endif // FIXEDFIELDDEFINITION_H
