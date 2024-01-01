/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef FIXEDFILEDEFINITION_H
#define FIXEDFILEDEFINITION_H
#include <QList>
#include <QHash>
#include <QString>
#include "src/fixedfielddefinition.h"
#include "src/fieldcheckitem.h"


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
    //本文件的必填检查规则列表
    QList <fieldcheckitem> fieldcheckList;
    //主键字段清单
    QList <uint> primaryKeyFieldList;
    //所属配置文件
    QString fileIni;
    //使用的正则匹配文件名
    QString fileName;
    QString fileNameWithVersion;
    QString  configSegment;
    QString fileDescribe;
    int fieldLengthType;

    //编码信息
    QString ecoding;

    //1.9.20版本开始新增的FIXED文件增强校验功能
    QString firstrowcheck;
    QString lastrowcheck;
    int versioncheckrow=0;
    //0精确匹配,非0模糊匹配,该行存在版本号内容即可,兼容版本号行还要其他内容时的情况
    int versioncheckmode;
    QString version;
    int fieldcountcheckrow=0;
    int fielddetailcheckbeginrow=0;
    int rowcountcheckrow=0;


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
    int getFieldlengthtype() const;
    void setFieldlengthtype(const int &value);
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
    const QString &getFirstrowcheck() const;
    void setFirstrowcheck(const QString &newFirstrowcheck);
    const QString &getLastrowcheck() const;
    void setLastrowcheck(const QString &newLastrowcheck);
    int getVersioncheckrow() const;
    void setVersioncheckrow(int newVersioncheckrow);
    const QString &getVersion() const;
    void setVersion(const QString &newVersion);
    int getFieldcountcheckrow() const;
    void setFieldcountcheckrow(int newFieldcountcheckrow);
    int getFielddetailcheckbeginrow() const;
    void setFielddetailcheckbeginrow(int newFielddetailcheckbeginrow);
    int getRowcountcheckrow() const;
    void setRowcountcheckrow(int newRowcountcheckrow);
    int getVersioncheckmode() const;
    void setVersioncheckmode(int newVersioncheckmode);
    QList<fieldcheckitem> getFieldcheckList() const;
    void addFieldcheckItem(const fieldcheckitem &value);
    void clearFieldcheckItemList();
    void setFieldcheckItemList(const QList<fieldcheckitem> &value);
    const QString &getConfigSegment() const;
    void setConfigSegment(const QString &newConfigSegment);
    const QList<uint> &getPrimaryKeyFieldList() const;
    void setPrimaryKeyFieldList(const QList<uint> &newPrimaryKeyFieldList);
};

#endif // FIXEDFILEDEFINITION_H
