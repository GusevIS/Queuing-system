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

class QueuingSystem
{
public:
  QueuingSystem(int numberOfSources, int workIntensity,
                int numberOfDevices, int serviceIntensity, unsigned int bufferSize);
  void startSystem();
  Request chooseEarliestRequest(std::vector<Request> requestsToBuffer) const;
  Device chooseDevice(std::vector<Device> devices) const;

private:
  bool systemIsActive;
  std::vector<Source> sources_;
  std::vector<Device> devices_;
  Buffer buffer_;
  double currentTime_;
};

#endif // QUEUINGSYSTEM_H
