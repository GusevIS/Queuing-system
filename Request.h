#ifndef REQUEST_H
#define REQUEST_H

struct Request
{
  int sourceNumber;
  double generationTime;
  double waitingTime;
};

struct ResultSet
{
  int sourceNumber;
  double probabilityOfFailure;
  int processedReqNumber;
  int failedRequestsNumber;
  int generateReqNumber;
  double bufferTime;
  double systemTime;
  double processingTime;
};

#endif // REQUEST_H
