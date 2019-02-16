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
#include "faultcause.h"

FaultCause::FaultCause()
{

}

int FaultCause::getConfigIndex() const
{
    return configIndex;
}

void FaultCause::setConfigIndex(int value)
{
    configIndex = value;
}

QString FaultCause::getCause() const
{
    return cause;
}

void FaultCause::setCause(const QString &value)
{
    cause = value;
}
