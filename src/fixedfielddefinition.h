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
#ifndef FIXEDFIELDDEFINITION_H
#define FIXEDFIELDDEFINITION_H

#include <QString>

class FixedFieldDefinition
{
public:
    FixedFieldDefinition();

    QString getFieldType() const;
    void setFieldType(const QString &value);

    int getRowBeginIndex() const;
    void setRowBeginIndex(int value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFieldName() const;
    void setFieldName(const QString &value);

    QString getFieldDescribe() const;
    void setFieldDescribe(const QString &value);

private:
    QString fieldType;
    int rowBeginIndex;
    int length;
    int decLength;
    QString fieldName;
    QString fieldDescribe;
};

#endif // FIXEDFIELDDEFINITION_H
