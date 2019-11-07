#include "event.h"

Event::Event(EventType eventType, double currentTime):
  eventType_(eventType),
  systemTime_(currentTime)
{

}

EventType Event::getEventType() const
{
    return eventType_;
}

void Event::setEventType(const EventType &value)
{
    eventType_ = value;
}

double Event::getSystemTime() const
{
    return systemTime_;
}

void Event::setSystemTime(double systemTime)
{
    systemTime_ = systemTime;
}

std::string Event::getChangeLog() const
{
    return changeLog_;
}

void Event::setChangeLog(const std::string &changeLog)
{
    changeLog_ = changeLog;
}

int Event::getNumberOfRequestsInBuffer() const
{
    return numberOfRequestsInBuffer_;
}

void Event::setNumberOfRequestsInBuffer(int numberOfRequestsInBuffer)
{
    numberOfRequestsInBuffer_ = numberOfRequestsInBuffer;
}

bool Event::getDeviceIsBusy() const
{
    return deviceIsBusy_;
}

void Event::setDeviceIsBusy(bool deviceIsBusy)
{
    deviceIsBusy_ = deviceIsBusy;
}
