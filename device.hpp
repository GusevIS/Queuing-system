#ifndef DEVICE_H
#define DEVICE_H

#include <cmath>

class Device
{
public:
  Device(int deviceNumber);
  double calculateServiceTime(double currentTime);
  bool isFree(double currentTime) const;
  double getReleaseTime() const;
  void setReleaseTime(double time);

private:
  double releaseTime_;
  const int deviceNumber_;
};

#endif // DEVICE_H
