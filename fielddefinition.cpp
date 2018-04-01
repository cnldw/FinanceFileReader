#include "fielddefinition.h"

FieldDefinition::FieldDefinition()
{

}

QString FieldDefinition::getFiledType() const
{
    return filedType;
}

void FieldDefinition::setFiledType( QString value)
{
    filedType = value;
}

int FieldDefinition::getLength() const
{
    return length;
}

void FieldDefinition::setLength(int value)
{
    length = value;
}

int FieldDefinition::getDecLength() const
{
    return decLength;
}

void FieldDefinition::setDecLength(int value)
{
    decLength = value;
}

QString FieldDefinition::getFiledName() const
{
    return filedName;
}

void FieldDefinition::setFiledName( QString value)
{
    filedName = value;
}

QString FieldDefinition::getFiledDescribe() const
{
    return filedDescribe;
}

void FieldDefinition::setFiledDescribe( QString value)
{
    filedDescribe = value;
}
