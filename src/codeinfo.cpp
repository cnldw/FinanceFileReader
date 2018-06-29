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
#include "src/codeinfo.h"

CodeInfo::CodeInfo()
{

}

QString CodeInfo::getCode()const{
    return this->code;
}
QString CodeInfo::getVersion()const{
    return this->version;
}
QString CodeInfo::getName()const{
    return this->name;
}
void CodeInfo::setCode(QString code){
    this->code=code;
}
void CodeInfo::setVersion(QString version){
    this->version=version;
}
void CodeInfo::setName(QString name){
    this->name=name;
}
