/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "fixedfielddefinition.h"

FixedFieldDefinition::FixedFieldDefinition()
{

}

QString FixedFieldDefinition::getFieldType() const
{
    return fieldType;
}

void FixedFieldDefinition::setFieldType(const QString &value)
{
    fieldType = value;
}

int FixedFieldDefinition::getRowBeginIndex() const
{
    return rowBeginIndex;
}

void FixedFieldDefinition::setRowBeginIndex(int value)
{
    rowBeginIndex = value;
}

int FixedFieldDefinition::getLength() const
{
    return length;
}

void FixedFieldDefinition::setLength(int value)
{
    length = value;
}

int FixedFieldDefinition::getDecLength() const
{
    return decLength;
}

void FixedFieldDefinition::setDecLength(int value)
{
    decLength = value;
}

QString FixedFieldDefinition::getFieldName() const
{
    return fieldName;
}

void FixedFieldDefinition::setFieldName(const QString &value)
{
    fieldName = value;
}

QString FixedFieldDefinition::getFieldDescribe() const
{
    return fieldDescribe;
}

void FixedFieldDefinition::setFieldDescribe(const QString &value)
{
    fieldDescribe = value;
}

