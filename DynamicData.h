#ifndef DynamicData_h
#define DynamicData_h

#include <Arduino.h>
#include "NVMData.h"

class DynamicData
{
public:
    static const int numberOfErrorMessageHist = 4;
private:
    DynamicData(/* args */);
    unsigned int errorCounter = 0;
    String errorMessageHist[numberOfErrorMessageHist];
    int errorMessagePointer = 0;
public:
    static DynamicData& get()
    {
        static DynamicData globalDynamicData;
        return globalDynamicData;
    }
    void Init();
    // static definitions
    const int MAX_NO_UPDATE = 600;

    String ipaddress = "";
    
    unsigned long epochTime = 0;
    unsigned int uptimeHours = 0;
    int connections = 0;
    bool setNewNetwork = false;
    int scene = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    int waittime = 10;
    boolean connected = false;

    String setToZeroReason = "Startup";
    
    String getHour(unsigned long actTime);
    String getMinute(unsigned long actTime);
    String getSecond(unsigned long actTime);
    String getFormattedTime();
    int getDay(unsigned long epochTime) const;
    int getHours(unsigned long epochTime) const;
    int getMinutes(unsigned long epochTime) const;
    int getSeconds(unsigned long epochTime) const;
    unsigned int getErrorCounter();
    void incErrorCounter(String message);
    String getErrorHist(int pos);
};

void DynamicData::Init() {
    for (int i = 0; i < numberOfErrorMessageHist; i++)
    {
        errorMessageHist[i] = "";
    }
}
unsigned int DynamicData::getErrorCounter()
{
    return errorCounter;
    this->errorCounter;
}
void DynamicData::incErrorCounter(String message)
{
    errorMessageHist[errorMessagePointer] = message;
    errorMessagePointer++;
    if (errorMessagePointer >= numberOfErrorMessageHist)
    {
        errorMessagePointer = 0;
    }
    errorCounter++;
}
String DynamicData::getErrorHist(int pos)
{
    int arrayPos = pos + errorMessagePointer;
    if (arrayPos >= numberOfErrorMessageHist)
    {
        arrayPos -= numberOfErrorMessageHist;
    }
    if (arrayPos >= numberOfErrorMessageHist)
    {
        return "end";
    }
    return errorMessageHist[arrayPos];
}
int DynamicData::getDay(unsigned long epochTime) const {
  return (((epochTime  / 86400L) + 4 ) % 7); //0 is Sunday
}
int DynamicData::getHours(unsigned long epochTime) const {
  return ((epochTime  % 86400L) / 3600);
}
int DynamicData::getMinutes(unsigned long epochTime) const {
  return ((epochTime % 3600) / 60);
}
int DynamicData::getSeconds(unsigned long epochTime) const {
  return (epochTime % 60);
}

DynamicData::DynamicData(/* args */)
{
}

String DynamicData::getFormattedTime()
{
    unsigned long actTime = this->epochTime;
    String retVal = DynamicData::getHour(actTime);
    retVal += ":";
    retVal += DynamicData::getMinute(actTime);
    retVal += ":";
    retVal += DynamicData::getSecond(actTime);
    return retVal;
}
String DynamicData::getHour(unsigned long actTime)
{
    String retVal = " ";
    int hour = this->getHours(actTime);
    if (hour < 10)
    {
        retVal += String(hour);
    }
    else
    {
        retVal = String(hour);
    }
    return retVal;
}
String DynamicData::getMinute(unsigned long actTime)
{
    String retVal = "0";
    int minute = this->getMinutes(actTime);
    if (minute < 10)
    {
        retVal += String(minute);
    }
    else
    {
        retVal = String(minute);
    }
    return retVal;
}
String DynamicData::getSecond(unsigned long actTime)
{
    String retVal = "0";
    int second = this->getSeconds(actTime);
    if (second < 10)
    {
        retVal += String(second);
    }
    else
    {
        retVal = String(second);
    }
    return retVal;
}
#endif
