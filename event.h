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

class Event
{
public:
  Event(EventType eventType, double currentTime, std::string changeLog, int numberRequests, std::vector<bool> devicesStatus);

  EventType getEventType() const;
  void setEventType(const EventType &value);

  double getSystemTime() const;
  void setSystemTime(double systemTime);

  std::string getChangeLog() const;
  void setChangeLog(const std::string &changeLog);

  int getNumberOfRequestsInBuffer() const;
  void setNumberOfRequestsInBuffer(int numberOfRequestsInBuffer);

  bool getDeviceIsBusy() const;
  void setDeviceIsBusy(bool deviceIsBusy);

private:
  EventType eventType_;
  double systemTime_;

  std::string changeLog_;
  int numberOfRequestsInBuffer_;
  std::vector<bool> devicesIsBusy_;


};

#endif // EVENT_H
