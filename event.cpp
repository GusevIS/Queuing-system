#include "event.h"

Event::Event(EventType eventType, double currentTime, std::string changeLog, int numberRequests, std::vector<DeviceStatus> devicesStatus,
             std::vector<SourceStatus> sourcesStatuses, std::vector<BufferRequest> bufferStatus):
  eventType_(eventType),
  systemTime_(currentTime),
  changeLog_(changeLog),
  numberOfRequestsInBuffer_(numberRequests),
  sourcesStatuses_(sourcesStatuses),
  devicesIsBusy_(devicesStatus),
  bufferStatus_(bufferStatus)
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

std::vector<DeviceStatus> Event::getDevicesIsBusy() const
{
  return devicesIsBusy_;
}

void Event::setDeviceIsBusy(std::vector<DeviceStatus> devicesIsBusy)
{
  devicesIsBusy_ = devicesIsBusy;
}

std::vector<SourceStatus> Event::getSourcesStatuses() const
{
  return sourcesStatuses_;
}

void Event::setSourcesStatuses(const std::vector<SourceStatus> &value)
{
  sourcesStatuses_ = value;
}

std::vector<BufferRequest> Event::getBufferStatus() const
{
    return bufferStatus_;
}

void Event::setBufferStatus(const std::vector<BufferRequest> &bufferStatus)
{
    bufferStatus_ = bufferStatus;
}
