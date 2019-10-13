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
#ifndef OFDFILEDEFINITION_H
#define OFDFILEDEFINITION_H

#include <QList>
#include <QString>
#include "src/ofdfielddefinition.h"

class OFDFileDefinition
{

private:

    //配置是否可用
    bool useAble;
    //配置不可用原因
    QString message;
    //字段总和
    int fieldCount;
    //字典类型
    QString dictionary;
    //每行长度
    int rowLength;
    //记录总行数的记录数的长度
    int rowCountLength;
    // 文件说明
    QString describe;
    //字段信息
    QList <OFDFieldDefinition> fieldList;

public:
    OFDFileDefinition();
    ~OFDFileDefinition();
    bool getUseAble()const;
    QString getMessage()const;
    int getFieldCount()const;
    int getRowLength()const;
    QList <OFDFieldDefinition>  getFieldList()const;
    void setFieldCount(int count);
    void setRowLength(int length);
    void setUseAble(bool useAble);
    void setMessage(QString message);
    void setFieldList(QList <OFDFieldDefinition> list);

    int getRowCountLength() const;
    void setRowCountLength(int value);
    QString getDescribe() const;
    void setDescribe(const QString &value);
    QString getDictionary() const;
    void setDictionary(const QString &value);
};

#endif // OFDFILEDEFINITION_H
