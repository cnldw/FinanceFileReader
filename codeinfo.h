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
#ifndef CODEINFO_H
#define CODEINFO_H

#include <QString>

class CodeInfo
{
public:
    CodeInfo();
    QString getCode()const;
    QString getVersion()const;
    QString getName()const;
    void setCode(QString name);
    void setVersion(QString version);
    void setName(QString name);

private:
    QString code;
    QString version;
    QString name;
};

#endif // CODEINFO_H
