/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef CSVFIELDDEFINITION_H
#define CSVFIELDDEFINITION_H
#include <QString>

/**
 * @brief The CsvFieldDefinition class CSV文件的字段定义
 */
class CsvFieldDefinition
{
public:
    CsvFieldDefinition();

    QString getFieldDescribe() const;
    void setFieldDescribe(const QString &value);

    const QString &getFieldName() const;
    void setFieldName(const QString &newFieldName);
    int getIsNumber() const;
    void setIsNumber(int newIsNumber);
    int getDecimalPointShift() const;
    void setDecimalPointShift(int newDecimalPointShift);

private:
    QString fieldDescribe;
    QString fieldName;
    // -1未说明,0不是,1是
    int isNumber=-1;
    //小数点移位大于等于0,位移2位则代表数据缩小100倍,比如将单位从分转化为元
    int decimalPointShift=0;
};

#endif // CSVFIELDDEFINITION_H
