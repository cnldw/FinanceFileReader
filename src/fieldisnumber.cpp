/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "fieldisnumber.h"

int FieldIsNumber::getDecimalLength() const
{
    return decimalLength;
}

void FieldIsNumber::setDecimalLength(int value)
{
    decimalLength = value;
}

bool FieldIsNumber::getIsNumber() const
{
    return isNumber;
}

void FieldIsNumber::setIsNumber(bool value)
{
    isNumber = value;
}

FieldIsNumber::FieldIsNumber()
{

}
