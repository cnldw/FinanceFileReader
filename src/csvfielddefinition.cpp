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
