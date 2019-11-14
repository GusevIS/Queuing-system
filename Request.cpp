#include "Request.h"

Request::Request(int sourceNumber, double generationTime):
  sourceNumber_(sourceNumber),
  generationTime_(generationTime),
  bufferArriveTime_(0),
  waitingTime_(0),
  processedTime_(0)
{

}

void Request::calculateWaitingTime(double currentTime)
{
  waitingTime_ = currentTime - bufferArriveTime_;
}

int Request::getSourceNumber() const
{
  return sourceNumber_;
}

double Request::getGenerationTime() const
{
  return generationTime_;
}

void Request::setBufferArriveTime(double bufferArriveTime)
{
  bufferArriveTime_ = bufferArriveTime;
}

double Request::getBufferArriveTime() const
{
  return bufferArriveTime_;
}

void Request::setWaitingTime(double waitingTime)
{
  waitingTime_ = waitingTime;
}

double Request::getWaitingTime() const
{
  return waitingTime_;
}
