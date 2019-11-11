#ifndef SOURCE_H
#define SOURCE_H

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include "Request.h"

const std::string idleStatus = "idle";
const std::string generateStatus = "generate request";

class Source
{
public:
  Source(double lambda, int sourceNumber);
  Request generateRequest(double currentTime);
  void increaseDeniedRequestsCount();
  void increaseProcessedRequestCount();
  int getDeniedRequestsCount() const;
  void setDeniedRequestsCount(int deniedRequestsCount);
  int getProcessedRequestsCount() const;
  void setProcessedRequestsCount(int processedRequestsCount);
  int getSourceNumber() const;
  int getRequestCount() const;
  void addBufferTime(double bufferTime);
  double getBufferTime() const;
  void addProcessingTime(double processingTime);
  double getProcessingTime() const;


  std::string getStatus() const;
  void setStatus(const std::string &status);

private:
  double lambda_;
  int requestsCount_;
  const int sourceNumber_;
  int deniedRequestsCount_;
  int processedRequestCount_;
  double bufferTime_;
  double processingTime_;
};

#endif // SOURCE_H
