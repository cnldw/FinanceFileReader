/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef CSVFILEDEFINITION_H
#define CSVFILEDEFINITION_H
#include <QString>
#include <QList>
#include "src/csvfielddefinition.h"
#include "src/fieldcheckitem.h"

/**
 * @brief The CsvFileDefinition class CSV文件类的定义
 */
class CsvFileDefinition
{
private:
    //使用的正则匹配文件名
    QString fileName;
    //文件名，带字段和，解析器名字
    QString fileNameWithCount;
    QString  configSegment;
    //文件解释
    QString fileDescribe;
    //配置是否可用
    bool useAble;
    //配置不可用原因
    QString message;
    //分隔符
    QString split;
    //是不是使用分隔符做行尾部
    QString endwithflag;
    //标题行
    int titlerowindex;
    //数据起始行
    int databeginrowindex;
    //文件尾部忽略的行--1自动
    int endIgnoreRow;
    //编码信息
    QString ecoding;
    //自动编码标示
    bool autoecoding=false;
    //字段总和
    int fieldCount;
    //所属配置文件
    QString fileIni;
    //是否清除双引号边界符
    bool clearQuotes;
    //是否对字段trim，有些固定分隔符文件同时也是定长的，需要trim
    bool trim;
    //字段信息
    QList <CsvFieldDefinition> fieldList;
    //1.9.20版本开始新增的CSV文件增强校验功能
    int versioncheckrow=0;
    //0精确匹配,非0模糊匹配,该行存在版本号内容即可,兼容版本号行还要其他内容时的情况
    int versioncheckmode=0;
    QString version="";

    QString firstrowcheck="";
    QString lastrowcheck="";

    bool ffAuto=false;

    //本文件的必填检查规则列表
    QList <fieldcheckitem> fieldcheckList;
    //主键字段清单
    QList <uint> primaryKeyFieldList;


public:
    CsvFileDefinition();
    bool getUseAble() const;
    void setUseAble(bool value);
    QString getMessage() const;
    void setMessage(const QString &value);
    int getFieldCount() const;
    void setFieldCount(int value);
    QList<CsvFieldDefinition> getFieldList() const;
    void setFieldList(const QList<CsvFieldDefinition> &value);
    QString getFileName() const;
    void setFileName(const QString &value);
    QString getSplit() const;
    void setSplit(const QString &value);
    int getDatabeginrowindex() const;
    void setDatabeginrowindex(int value);
    QString getEcoding() const;
    void setEcoding(const QString &value);
    int getTitlerowindex() const;
    void setTitlerowindex(int value);
    QString getFileIni() const;
    void setFileIni(const QString &value);
    QString getEndwithflag() const;
    void setEndwithflag(const QString &value);
    QString getFileDescribe() const;
    void setFileDescribe(const QString &value);
    QString getFileNameWithCount() const;
    void setFileNameWithCount(const QString &value);
    int getEndIgnoreRow() const;
    void setEndIgnoreRow(int value);
    bool getClearQuotes() const;
    void setClearQuotes(bool value);
    bool getFfAuto() const;
    void setFfAuto(bool value);
    bool getTrim() const;
    void setTrim(bool value);
    int getVersioncheckrow() const;
    void setVersioncheckrow(int newVersioncheckrow);
    int getVersioncheckmode() const;
    void setVersioncheckmode(int newVersioncheckmode);
    const QString &getVersion() const;
    void setVersion(const QString &newVersion);
    const QString &getFirstrowcheck() const;
    void setFirstrowcheck(const QString &newFirstrowcheck);
    const QString &getLastrowcheck() const;
    void setLastrowcheck(const QString &newLastrowcheck);
    QList<fieldcheckitem> getFieldcheckList() const;
    void addFieldcheckItem(const fieldcheckitem &value);
    const QString &getConfigSegment() const;
    void setConfigSegment(const QString &newConfigSegment);
    void clearFieldcheckItemList();
    void setFieldcheckItemList(const QList<fieldcheckitem> &value);
    bool getAutoecoding() const;
    void setAutoecoding(bool newAutoecoding);
    const QList<uint> &getPrimaryKeyFieldList() const;
    void setPrimaryKeyFieldList(const QList<uint> &newPrimaryKeyFieldList);
};

#endif // CSVFILEDEFINITION_H
