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
