#include "utils.h"

Utils::Utils()
{

}
bool Utils::isDirExist(QString fullPath)
{
    if(QDir(fullPath).exists())
    {
        return true;
    }
    return false;
}

bool Utils::isFileExist(QString fullFileName)
{
    if(QFileInfo(fullFileName).isFile())
    {
        return true;
    }
    return false;
}

QString Utils::getVersion()
{
    return "V1.1";
}

