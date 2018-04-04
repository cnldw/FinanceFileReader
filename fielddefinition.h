#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include<QString>

class FieldDefinition
{
public:
    FieldDefinition();

    QString getFiledType() const;
    void setFiledType( QString value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFiledName() const;
    void setFiledName( QString value);

    QString getFiledDescribe() const;
    void setFiledDescribe( QString value);

    int getRowBeginIndex() const;
    void setRowBeginIndex(int value);

private:
    QString filedType;
    int rowBeginIndex;
    int length;
    int decLength;
    QString filedName;
    QString filedDescribe;
};

#endif // FIELDDEFINITION_H
