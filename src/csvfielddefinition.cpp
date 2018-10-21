#include "csvfielddefinition.h"

CsvFieldDefinition::CsvFieldDefinition()
{

}

QString CsvFieldDefinition::getFieldName() const
{
    return fieldName;
}

void CsvFieldDefinition::setFieldName(const QString &value)
{
    fieldName = value;
}
