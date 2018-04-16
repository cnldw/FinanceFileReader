#include "codeinfo.h"

CodeInfo::CodeInfo()
{

}

QString CodeInfo::getCode()const{
    return this->code;
}
QString CodeInfo::getVersion()const{
    return this->version;
}
QString CodeInfo::getName()const{
    return this->name;
}
void CodeInfo::setCode(QString code){
    this->code=code;
}
void CodeInfo::setVersion(QString version){
    this->version=version;
}
void CodeInfo::setName(QString name){
    this->name=name;
}
