#include "ofdfiledefinition.h"


OFDFileDefinition::OFDFileDefinition()
{

}

int OFDFileDefinition::getfieldCount()const
{
    return this->fieldCount;
}
int OFDFileDefinition::getrowLength()const
{
    return this->rowLength;
}
bool OFDFileDefinition::getuseAble()const{
    return this->useAble;
}
QString OFDFileDefinition::getMessage()const{
    return this->message;
}
QList <FieldDefinition>  OFDFileDefinition::getfieldList()const
{
    return this->fieldList;
}
void OFDFileDefinition::setfieldCount(int count)
{
    this->fieldCount=count;
}
void OFDFileDefinition::setrowLength(int length)
{
    this->rowLength=length;
}
void  OFDFileDefinition::setfieldList(QList <FieldDefinition> list)
{
    this->fieldList=list;
}

void OFDFileDefinition::setuseAble(bool useAble){
    this->useAble=useAble;
}
void OFDFileDefinition::setMessage(QString message){
    this->message=message;
}

OFDFileDefinition::~OFDFileDefinition()
{
}
