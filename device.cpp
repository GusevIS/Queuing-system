#include "device.hpp"

const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

Device::Device(int deviceNumber):
  releaseTime_(0),
  deviceNumber_(deviceNumber)
{

}

double Device::calculateServiceTime(double currentTime)
{
  double randNumber = 0;
  while (randNumber == 1.0 || randNumber == 0.0)
    randNumber = rand() * fraction;
  double serviceTime = (randNumber * (5 - 0) + 0);
  releaseTime_ = currentTime + serviceTime;
  return serviceTime;
}

bool Device::isFree(double currentTime) const
{
  return currentTime >= releaseTime_;
}

double Device::getReleaseTime() const
{
  return releaseTime_;
}

void Device::setReleaseTime(double time)
{
  releaseTime_ = time;
}

int Device::getDeviceNumber() const
{
  return deviceNumber_;
}
