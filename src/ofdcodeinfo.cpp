/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the LGPL-3.0;
************************************************************************/
#include "src/ofdcodeinfo.h"

OFDCodeInfo::OFDCodeInfo()
{

}

QString OFDCodeInfo::getCode()const{
    return this->code;
}
QString OFDCodeInfo::getName()const{
    return this->name;
}
void OFDCodeInfo::setCode(QString code){
    this->code=code;
}
void OFDCodeInfo::setName(QString name){
    this->name=name;
}
