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
