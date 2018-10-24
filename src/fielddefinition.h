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
#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>

class FieldDefinition
{
public:
    FieldDefinition();

    QString getFieldType() const;
    void setFieldType( QString value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFiledName() const;
    void setFiledName( QString value);

    QString getFiledDescribe() const;
    void setFiledDescribe( QString value);

    int getRowBeginIndex() const;
    void setRowBeginIndex(int value);

private:
    QString fieldType;
    int rowBeginIndex;
    int length;
    int decLength;
    QString filedName;
    QString filedDescribe;
};

#endif // FIELDDEFINITION_H
