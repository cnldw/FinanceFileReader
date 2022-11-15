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
#ifndef CSVFAULTCAUSE_H
#define CSVFAULTCAUSE_H
#include <QString>
#include "src/configfile.h"

class FaultCause
{
public:
    FaultCause();

    int getConfigIndex() const;
    void setConfigIndex(int value);

    QString getCause() const;
    void setCause(const QString &value);

    const matchIndex &getConfigIndex2() const;
    void setConfigIndex2(const matchIndex &newConfigIndex2);

private:
    int configIndex;
    matchIndex configIndex2;
    QString cause;
};

#endif // CSVFAULTCAUSE_H
