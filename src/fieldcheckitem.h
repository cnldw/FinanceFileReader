/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
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
