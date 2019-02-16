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
