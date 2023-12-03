/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#ifndef OFDFIELDDEFINITION_H
#define OFDFIELDDEFINITION_H

#include <QString>

class OFDFieldDefinition
{
public:
    OFDFieldDefinition();

    QString getFieldType() const;
    void setFieldType( QString value);

    int getRowBeginIndex() const;
    void setRowBeginIndex(int value);

    int getLength() const;
    void setLength(int value);

    int getDecLength() const;
    void setDecLength(int value);

    QString getFieldName() const;
    void setFieldName( QString value);

    QString getFieldDescribe() const;
    void setFieldDescribe( QString value);

private:
    QString fieldType;
    int rowBeginIndex;
    int length;
    int decLength;
    QString fieldName;
    QString fieldDescribe;
};

#endif // OFDFIELDDEFINITION_H
