#ifndef FIELDISNUMBER_H
#define FIELDISNUMBER_H


class FieldIsNumber
{

private:
 bool isNumber;
 int decimalLength;

public:
    FieldIsNumber();
    int getDecimalLength() const;
    void setDecimalLength(int value);
    bool getIsNumber() const;
    void setIsNumber(bool value);
};

#endif // FIELDISNUMBER_H
