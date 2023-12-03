/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef FIELDISNUMBER_H
#define FIELDISNUMBER_H


class FieldIsNumber
{

private:
 bool isNumber=false;
 int decimalLength=0;

public:
    FieldIsNumber();
    int getDecimalLength() const;
    void setDecimalLength(int value);
    bool getIsNumber() const;
    void setIsNumber(bool value);
};

#endif // FIELDISNUMBER_H
