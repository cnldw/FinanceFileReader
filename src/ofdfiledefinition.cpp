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


OFDFileDefinition::OFDFileDefinition()
{

}

int OFDFileDefinition::getfieldCount()const
{
    return this->fieldCount;
}
int OFDFileDefinition::getrowLength()const
{
    return this->rowLength;
}
bool OFDFileDefinition::getuseAble()const{
    return this->useAble;
}
QString OFDFileDefinition::getMessage()const{
    return this->message;
}
QList <FieldDefinition>  OFDFileDefinition::getfieldList()const
{
    return this->fieldList;
}
void OFDFileDefinition::setfieldCount(int count)
{
    this->fieldCount=count;
}
void OFDFileDefinition::setrowLength(int length)
{
    this->rowLength=length;
}
void  OFDFileDefinition::setfieldList(QList <FieldDefinition> list)
{
    this->fieldList=list;
}

void OFDFileDefinition::setuseAble(bool useAble){
    this->useAble=useAble;
}
void OFDFileDefinition::setMessage(QString message){
    this->message=message;
}

OFDFileDefinition::~OFDFileDefinition()
{
}
