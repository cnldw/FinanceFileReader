#ifndef CSVFILEDEFINITION_H
#define CSVFILEDEFINITION_H
#include <QString>
#include <QList>
#include "src/csvfielddefinition.h"

class CsvFileDefinition
{
private:
    //使用的正则匹配文件名
    QString fileName;
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
    //编码信息
    QString ecoding;
    //字段总和
    int fieldCount;
    //所属配置文件
    QString fileIni;
    //字段信息
    QList <CsvFieldDefinition> fieldList;

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
};

#endif // CSVFILEDEFINITION_H
