#include "device.hpp"

const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

Device::Device(int deviceNumber, int alpha, int beta):
  releaseTime_(0),
  deviceNumber_(deviceNumber),
  alpha_(alpha),
  beta_(beta),
  status_(freeStatus)
{

}

void Device::updateStatus(double currentTime)
{
  if(currentTime > releaseTime_){
    status_ = freeStatus;
    std::cout << deviceNumber_ << "  -  " << status_ << std::endl;
  }
  std::cout << deviceNumber_ << "  +  " << status_ << std::endl;
}

double Device::calculateServiceTime(double currentTime, Request request)
{
  double randNumber = 0;
  while (randNumber == 1.0 || randNumber == 0.0)
    randNumber = rand() * fraction;
  double serviceTime = (randNumber * (alpha_ - beta_) + beta_);
  releaseTime_ = currentTime + serviceTime;
  status_ = busyStatus + "req from SOURCE " + std::to_string(request.getSourceNumber()) + " until " + std::to_string(releaseTime_);
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

std::string Device::getStatus() const
{
  std::cout << status_ << std::endl;
  return status_;
}

void Device::setStatus(const std::string &status)
{
  status_ = status;
}

void Device::setFreeStatus()
{
  status_ = freeStatus;
}
