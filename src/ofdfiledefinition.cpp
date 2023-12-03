/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/ofdfiledefinition.h"


int OFDFileDefinition::getRowCountLength() const
{
    return rowCountLength;
}

void OFDFileDefinition::setRowCountLength(int value)
{
    rowCountLength = value;
}

QString OFDFileDefinition::getDescribe() const
{
    return describe;
}

void OFDFileDefinition::setDescribe(const QString &value)
{
    describe = value;
}

QString OFDFileDefinition::getDictionary() const
{
    return dictionary;
}

void OFDFileDefinition::setDictionary(const QString &value)
{
    dictionary = value;
}

QString OFDFileDefinition::getConfigSegment() const
{
    return configSegment;
}

void OFDFileDefinition::setConfigSegment(const QString &value)
{
    configSegment = value;
}

QList<fieldcheckitem> OFDFileDefinition::getFieldcheckList() const
{
    return fieldcheckList;
}

void OFDFileDefinition::addFieldcheckItem(const fieldcheckitem &value)
{
    fieldcheckList.append(value);
}

void OFDFileDefinition::clearFieldcheckItemList(){
    fieldcheckList.clear();
}

void OFDFileDefinition::setFieldcheckItemList(const QList<fieldcheckitem> &value){
    fieldcheckList=value;
}

const QString &OFDFileDefinition::getUseForVersionAndType() const
{
    return useForVersionAndType;
}

void OFDFileDefinition::setUseForVersionAndType(const QString &newUseForVersionAndType)
{
    useForVersionAndType = newUseForVersionAndType;
}

OFDFileDefinition::OFDFileDefinition()
{
    
}

int OFDFileDefinition::getFieldCount()const
{
    return this->fieldCount;
}
int OFDFileDefinition::getRowLength()const
{
    return this->rowLength;
}
bool OFDFileDefinition::getUseAble()const{
    return this->useAble;
}
QString OFDFileDefinition::getMessage()const{
    return this->message;
}
QList <OFDFieldDefinition>  OFDFileDefinition::getFieldList()const
{
    return this->fieldList;
}
void OFDFileDefinition::setFieldCount(int count)
{
    this->fieldCount=count;
}
void OFDFileDefinition::setRowLength(int length)
{
    this->rowLength=length;
}
void  OFDFileDefinition::setFieldList(QList <OFDFieldDefinition> list)
{
    this->fieldList=list;
}

void OFDFileDefinition::setUseAble(bool useAble){
    this->useAble=useAble;
}
void OFDFileDefinition::setMessage(QString message){
    this->message=message;
}

OFDFileDefinition::~OFDFileDefinition()
{
}
