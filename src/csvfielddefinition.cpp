/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "csvfielddefinition.h"

CsvFieldDefinition::CsvFieldDefinition()
{

}

QString CsvFieldDefinition::getFieldDescribe() const
{
    return fieldDescribe;
}

void CsvFieldDefinition::setFieldDescribe(const QString &value)
{
    fieldDescribe = value;
}

const QString &CsvFieldDefinition::getFieldName() const
{
    return fieldName;
}

void CsvFieldDefinition::setFieldName(const QString &newFieldName)
{
    fieldName = newFieldName;
}

int CsvFieldDefinition::getIsNumber() const
{
    return isNumber;
}

void CsvFieldDefinition::setIsNumber(int newIsNumber)
{
    isNumber = newIsNumber;
}

int CsvFieldDefinition::getDecimalPointShift() const
{
    return decimalPointShift;
}

void CsvFieldDefinition::setDecimalPointShift(int newDecimalPointShift)
{
    decimalPointShift = newDecimalPointShift;
}
