#ifndef DEVICE_H
#define DEVICE_H

#include <cmath>
#include <iostream>

#include "Request.h"

const std::string freeStatus = "free";
const std::string busyStatus = "service ";

class Device
{
public:
  Device(int deviceNumber, int alpha, int beta);
  double calculateServiceTime(double currentTime, Request request);
  bool isFree(double currentTime) const;
  double getReleaseTime() const;
  void setReleaseTime(double time);
  int getDeviceNumber() const;

  std::string getStatus() const;
  void setStatus(const std::string &status);

  void updateStatus(double currentTime);

  double getTotalServiceTime() const;
  void setTotalServiceTime(double totalServiceTime);

  double getServiceCoefficient() const;
  void setServiceCoefficient(double serviceCoefficient);

private:
  double releaseTime_;
  const int deviceNumber_;
  int alpha_, beta_;
  std::string status_;
  double totalServiceTime_;
  double serviceCoefficient_;
};

#endif // DEVICE_H
