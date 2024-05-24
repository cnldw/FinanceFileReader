/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef OFDFILEDEFINITION_H
#define OFDFILEDEFINITION_H

#include <QList>
#include <QString>
#include "src/ofdfielddefinition.h"
#include "src/fieldcheckitem.h"

class OFDFileDefinition
{

private:

    //配置是否可用
    bool useAble;
    QString  configSegment;
    QStringList  useForTA=QStringList();
    QString useForVersionAndType;
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
    //本文件的必填检查规则列表
    QList <fieldcheckitem> fieldcheckList;
    //主键字段清单
    QList <uint> primaryKeyFieldList;

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
    QString getConfigSegment() const;
    void setConfigSegment(const QString &value);
    QList<fieldcheckitem> getFieldcheckList() const;
    void addFieldcheckItem(const fieldcheckitem &value);
    void clearFieldcheckItemList();
    void setFieldcheckItemList(const QList<fieldcheckitem> &value);
    const QString &getUseForVersionAndType() const;
    void setUseForVersionAndType(const QString &newUseForVersionAndType);
    const QList<uint> &getPrimaryKeyFieldList() const;
    void setPrimaryKeyFieldList(const QList<uint> &newPrimaryKeyFieldList);
    const QStringList &getUseForTA() const;
    void setUseForTA(const QStringList &newUseForTA);
};

#endif // OFDFILEDEFINITION_H
