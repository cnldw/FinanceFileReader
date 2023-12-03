/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/ofdfielddefinition.h"

OFDFieldDefinition::OFDFieldDefinition()
{

}

QString OFDFieldDefinition::getFieldType() const
{
    return fieldType;
}

void OFDFieldDefinition::setFieldType( QString value)
{
    fieldType = value;
}

int OFDFieldDefinition::getLength() const
{
    return length;
}

void OFDFieldDefinition::setLength(int value)
{
    length = value;
}

int OFDFieldDefinition::getDecLength() const
{
    return decLength;
}

void OFDFieldDefinition::setDecLength(int value)
{
    decLength = value;
}

QString OFDFieldDefinition::getFieldName() const
{
    return fieldName;
}

void OFDFieldDefinition::setFieldName( QString value)
{
    fieldName = value;
}

QString OFDFieldDefinition::getFieldDescribe() const
{
    return fieldDescribe;
}

void OFDFieldDefinition::setFieldDescribe( QString value)
{
    fieldDescribe = value;
}

int OFDFieldDefinition::getRowBeginIndex() const
{
    return rowBeginIndex;
}

void OFDFieldDefinition::setRowBeginIndex(int value)
{
    rowBeginIndex = value;
}
