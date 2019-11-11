#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>

enum EventType
{
  SETTING_TO_BUFFER,
  SETTING_TO_DEVICE,
  FROM_SOURCE_TO_DEVICE
};

struct DeviceStatus
{
  int deviceNumber;
  std::string status;
};

struct SourceStatus
{
  int sourceNumber;
  bool isIdle;
};

class Event
{
public:
  Event(EventType eventType, double currentTime, std::string changeLog, int numberRequests, std::vector<DeviceStatus> devicesStatus,
        std::vector<SourceStatus> sourcesStatuses);

  EventType getEventType() const;
  void setEventType(const EventType &value);

  double getSystemTime() const;
  void setSystemTime(double systemTime);

  std::string getChangeLog() const;
  void setChangeLog(const std::string &changeLog);

  int getNumberOfRequestsInBuffer() const;
  void setNumberOfRequestsInBuffer(int numberOfRequestsInBuffer);

  std::vector<DeviceStatus> getDevicesIsBusy() const;
  void setDeviceIsBusy(std::vector<DeviceStatus> devicesIsBusy);

  std::vector<SourceStatus> getSourcesStatuses() const;
  void setSourcesStatuses(const std::vector<SourceStatus> &value);

private:
  EventType eventType_;
  double systemTime_;

  std::string changeLog_;
  int numberOfRequestsInBuffer_;
  std::vector<SourceStatus> sourcesStatuses_;
  std::vector<DeviceStatus> devicesIsBusy_;
};

#endif // EVENT_H
