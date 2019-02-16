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
#ifndef FIXEDFILEDEFINITION_H
#define FIXEDFILEDEFINITION_H
#include <QList>
#include <QHash>
#include <QString>
#include "src/fixedfielddefinition.h"


class FIXEDFileDefinition
{
private:
    //配置是否可用
    bool useAble;
    //配置不可用原因
    QString message;
    //字段总和--考虑到诸如NAV文件等每行的长度可能不一致，所以每行的字段数我们允许多种可能
    QList<int> fieldCountList;
    //每行最大字段数值
    int fieldCountMax;
    //每行长度--多种长度-存入方式为<行长度,行长度对应的列数>
    QHash<int,int> rowLengthHash;
    //数据起始行
    int dataRowBeginIndex;
    //文件尾部忽略的行
    int endIgnoreRow;
    //字段信息
    QList <FixedFieldDefinition> fieldList;
    //所属配置文件
    QString fileIni;
    //使用的正则匹配文件名
    QString fileName;
    QString fileNameWithVersion;
    QString fileDescribe;
    QString fieldLengthType;

    //编码信息
    QString ecoding;

public:
    FIXEDFileDefinition();
    ~FIXEDFileDefinition();
    bool getUseAble() const;
    void setUseAble(bool value);
    QString getMessage() const;
    void setMessage(const QString &value);
    QList<int> getFieldCountList() const;
    void setFieldCountList(const QList<int> &value);
    QList<FixedFieldDefinition> getFieldList() const;
    void setFieldList(const QList<FixedFieldDefinition> &value);
    QString getFileIni() const;
    void setFileIni(const QString &value);
    QString getFileName() const;
    void setFileName(const QString &value);
    QString getFileNameWithVersion() const;
    void setFileNameWithVersion(const QString &value);
    QString getFileDescribe() const;
    void setFileDescribe(const QString &value);
    QString getFieldlengthtype() const;
    void setFieldlengthtype(const QString &value);
    QString getEcoding() const;
    void setEcoding(const QString &value);
    int getDataRowBeginIndex() const;
    void setDataRowBeginIndex(int value);
    int getEndIgnoreRow() const;
    void setEndIgnoreRow(int value);
    QHash<int, int> getRowLengthHash() const;
    void setRowLengthHash(const QHash<int, int> &value);
    int getFieldCountMax() const;
    void setFieldCountMax(int value);
};

#endif // FIXEDFILEDEFINITION_H
