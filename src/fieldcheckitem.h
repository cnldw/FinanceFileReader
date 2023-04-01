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
#ifndef FIELDCHECKITEM_H
#define FIELDCHECKITEM_H

#include <QList>

struct CheckCondition{
    int index;
    QString value;
    QString valuewithdic;
};

class fieldcheckitem
{

    //条件清单,支持多个规则复用一个必填字段列表
    QList<QList <CheckCondition>> checkConditionListOverList;
    //必填字段清单
    QList <int> requiredFields;
public:
    fieldcheckitem();
    QList<int> getRequiredFields() const;
    void setRequiredFields(const QList<int> &value);
    const QList<QList<CheckCondition> > &getCheckConditionsListOverList() const;
    void addCheckConditions(const QList<CheckCondition> &newCheckConditionList);
    void setCheckConditions(QList<QList<CheckCondition>> &conListOverList);
};

#endif // FIELDCHECKITEM_H
