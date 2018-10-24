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
#include "src/fielddefinition.h"

FieldDefinition::FieldDefinition()
{

}

QString FieldDefinition::getFieldType() const
{
    return fieldType;
}

void FieldDefinition::setFieldType( QString value)
{
    fieldType = value;
}

int FieldDefinition::getLength() const
{
    return length;
}

void FieldDefinition::setLength(int value)
{
    length = value;
}

int FieldDefinition::getDecLength() const
{
    return decLength;
}

void FieldDefinition::setDecLength(int value)
{
    decLength = value;
}

QString FieldDefinition::getFiledName() const
{
    return filedName;
}

void FieldDefinition::setFiledName( QString value)
{
    filedName = value;
}

QString FieldDefinition::getFiledDescribe() const
{
    return filedDescribe;
}

void FieldDefinition::setFiledDescribe( QString value)
{
    filedDescribe = value;
}

int FieldDefinition::getRowBeginIndex() const
{
    return rowBeginIndex;
}

void FieldDefinition::setRowBeginIndex(int value)
{
    rowBeginIndex = value;
}
