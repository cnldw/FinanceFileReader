/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "dbffielddefinition.h"

DbfFieldDefinition::DbfFieldDefinition()
{

}

int DbfFieldDefinition::getFieldType() const
{
    return fieldType;
}

void DbfFieldDefinition::setFieldType(const int &value)
{
    fieldType = value;
}

int DbfFieldDefinition::getLength() const
{
    return length;
}

void DbfFieldDefinition::setLength(int value)
{
    length = value;
}

int DbfFieldDefinition::getDecLength() const
{
    return decLength;
}

void DbfFieldDefinition::setDecLength(int value)
{
    decLength = value;
}

QString DbfFieldDefinition::getFieldName() const
{
    return fieldName;
}

void DbfFieldDefinition::setFieldName(const QString &value)
{
    fieldName = value;
}

QString DbfFieldDefinition::getFieldDescribe() const
{
    return fieldDescribe;
}

void DbfFieldDefinition::setFieldDescribe(const QString &value)
{
    fieldDescribe = value;
}

QString DbfFieldDefinition::getFieldOriginName() const
{
    return fieldOriginName;
}

void DbfFieldDefinition::setFieldOriginName(const QString &value)
{
    fieldOriginName = value;
}
