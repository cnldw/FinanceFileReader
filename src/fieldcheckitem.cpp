/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "fieldcheckitem.h"

QList<int> fieldcheckitem::getRequiredFields() const
{
    return requiredFields;
}

void fieldcheckitem::setRequiredFields(const QList<int> &value)
{
    requiredFields = value;
}

const QList<QList<CheckCondition> > &fieldcheckitem::getCheckConditionsListOverList() const
{
    return checkConditionListOverList;
}

void fieldcheckitem::addCheckConditions(const QList<CheckCondition> &newCheckConditionList)
{
    checkConditionListOverList.append(newCheckConditionList);
}

void fieldcheckitem::setCheckConditions(QList<QList<CheckCondition>> &conListOverList){

    checkConditionListOverList=conListOverList;
}

fieldcheckitem::fieldcheckitem()
{

}
