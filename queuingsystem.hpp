#ifndef QUEUINGSYSTEM_H
#define QUEUINGSYSTEM_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include "buffer.hpp"
#include "device.hpp"
#include "source.hpp"

enum NextEvent
{
  SETTING_TO_BUFFER,
  SETTING_TO_DEVICE,
  FROM_SOURCE_TO_DEVICE
};

class QueuingSystem
{
public:
  QueuingSystem(int numberOfSources, int numberOfRequests,
                int numberOfDevices, double lambda, unsigned int bufferSize);
  void startSystem();
  Request chooseEarliestRequest(std::vector<Request> requestsToBuffer) const;
  Device chooseDevice(std::vector<Device> devices) const;
  NextEvent determineNextEvent(Request earliestRequest, Device device) const;

private:
  bool systemIsActive;
  std::vector<Source> sources_;
  std::vector<Device> devices_;
  Buffer buffer_;
  double currentTime_;
  const int numberOfRequests_;
};

#endif // QUEUINGSYSTEM_H
