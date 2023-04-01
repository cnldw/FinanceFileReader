/***********************************************************************
*Copyright [2018] [Liudewei(793554262@qq.com)]
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http:*www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
************************************************************************/
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QString>

struct matchIndex{
  int configFileIndex=0;
  int configSegmentIndex=0;
};

//dbf专用的匹配信息
struct dbfMatchInfo{
    QString ini;
    int configFileIndex;
    QString matchFileType;
    QString matchfileDescribe;
    float matchDegree;
};

template <typename T>
class ConfigFile {
private:
    QString fileName;
    QString author;
    QList<T> ConfigSegmentList;
public:
    const QString &getFileName() const
    {
        return fileName;
    }

    void setFileName(const QString &newFileName)
    {
        fileName = newFileName;
    }

    const QList<T> getConfigSegmentList() const
    {
        return ConfigSegmentList;
    }

    void setConfigSegmentList(const QList<T> &newConfigSegment)
    {
        ConfigSegmentList = newConfigSegment;
    }

    const QString getAuthor() const
    {
        return author;
    }

    void setAuthor(const QString &newAuthor)
    {
        author = newAuthor;
    }
};


#endif // CONFIGFILE_H
