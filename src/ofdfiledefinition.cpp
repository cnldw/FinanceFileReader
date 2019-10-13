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
#include "src/ofdfiledefinition.h"


int OFDFileDefinition::getRowCountLength() const
{
    return rowCountLength;
}

void OFDFileDefinition::setRowCountLength(int value)
{
    rowCountLength = value;
}

QString OFDFileDefinition::getDescribe() const
{
    return describe;
}

void OFDFileDefinition::setDescribe(const QString &value)
{
    describe = value;
}

QString OFDFileDefinition::getDictionary() const
{
    return dictionary;
}

void OFDFileDefinition::setDictionary(const QString &value)
{
    dictionary = value;
}

OFDFileDefinition::OFDFileDefinition()
{
    
}

int OFDFileDefinition::getFieldCount()const
{
    return this->fieldCount;
}
int OFDFileDefinition::getRowLength()const
{
    return this->rowLength;
}
bool OFDFileDefinition::getUseAble()const{
    return this->useAble;
}
QString OFDFileDefinition::getMessage()const{
    return this->message;
}
QList <OFDFieldDefinition>  OFDFileDefinition::getFieldList()const
{
    return this->fieldList;
}
void OFDFileDefinition::setFieldCount(int count)
{
    this->fieldCount=count;
}
void OFDFileDefinition::setRowLength(int length)
{
    this->rowLength=length;
}
void  OFDFileDefinition::setFieldList(QList <OFDFieldDefinition> list)
{
    this->fieldList=list;
}

void OFDFileDefinition::setUseAble(bool useAble){
    this->useAble=useAble;
}
void OFDFileDefinition::setMessage(QString message){
    this->message=message;
}

OFDFileDefinition::~OFDFileDefinition()
{
}
