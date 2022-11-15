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

fieldcheckitem::fieldcheckitem()
{

}
