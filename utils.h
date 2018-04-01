#ifndef UTILS_H
#define UTILS_H

#include<QString>
#include<QDir>
#include<QFileInfo>
namespace Ui {
class Utils;
}

class Utils
{
public:
    Utils();
     static bool isDirExist(QString fullPath);
     static bool isFileExist(QString fullFileName);
     static QString getVersion();
};

#endif // UTILS_H
