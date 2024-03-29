#ifndef REQUEST_H
#define REQUEST_H

class Request
{
public:
  Request(int sourceNumber, double generationTime);
  int getSourceNumber() const;
  double getGenerationTime() const;
  void calculateWaitingTime(double currentTime);
  void setBufferArriveTime(double bufferArriveTime);
  double getBufferArriveTime() const;
  void setWaitingTime(double waitingTime);
  double getWaitingTime() const;

private:
  int sourceNumber_;
  double generationTime_;
  double bufferArriveTime_;
  double waitingTime_;
  double processedTime_;
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
  double dispersionServTime;
  double dispersionWaitTime;
};

#endif // REQUEST_H
