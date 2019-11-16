#include "source.hpp"

const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

Source::Source(double lambda, int sourceNumber):
  lambda_(lambda),
  requestsCount_(0),
  sourceNumber_(sourceNumber),
  deniedRequestsCount_(0),
  processedRequestCount_(0)
{

}

Request Source::generateRequest(double currentTime)
{
  double randNumber = 0.0;
  while (randNumber == 1.0 || randNumber == 0.0 )
    randNumber = rand() * fraction;
  double generationTime = currentTime + (-log(randNumber)/lambda_);

  Request request(sourceNumber_, generationTime);
  requestsCount_++;

  return request;
}

void Source::increaseDeniedRequestsCount()
{
  deniedRequestsCount_++;
}

void Source::increaseProcessedRequestCount()
{
  processedRequestCount_++;
}

int Source::getDeniedRequestsCount() const
{
  return deniedRequestsCount_;
}

void Source::setDeniedRequestsCount(int deniedRequestsCount)
{
    deniedRequestsCount_ = deniedRequestsCount;
}

int Source::getProcessedRequestsCount() const
{
  return processedRequestCount_;
}

void Source::setProcessedRequestsCount(int processedRequestsCount)
{
  processedRequestCount_ = processedRequestsCount;
}

int Source::getSourceNumber() const
{
  return sourceNumber_;
}

int Source::getRequestCount() const
{
  return requestsCount_;
}

void Source::addBufferTime(double bufferTime)
{
  bufferTimes_.push_back(bufferTime);
}

std::list<double> Source::getBufferTime() const
{
  return bufferTimes_;
}

double Source::getAverageWaitingTime() const
{
  double totalWaitingTime = 0;
  for(auto waitingTime: bufferTimes_)
  {
    totalWaitingTime += waitingTime;
  }
  return totalWaitingTime / requestsCount_;
}

std::list<double> Source::getProcessingTime() const
{
  return processingTimes_;
}

void Source::addProcessingTime(double processingTime)
{
  processingTimes_.push_back(processingTime);
}

double Source::getAverageProcessingTime() const
{
    double totalProcessingTime = 0;
    for(auto processingTime: processingTimes_)
    {
      totalProcessingTime += processingTime;
    }
  return totalProcessingTime / processedRequestCount_;
}

double Source::getAverageSystemTime() const
{
  return getAverageWaitingTime() + getAverageProcessingTime();
}

double Source::getDispersionOfWaitingTime() const
{
  double dispersion = 0;
  double averageWaitingTime = getAverageWaitingTime();
  for(auto waitingTime: bufferTimes_)
  {
    dispersion += pow(waitingTime - averageWaitingTime, 2);
  }
  return dispersion / (bufferTimes_.size() - 1);
}

double Source::getDispersionOfProcessingTime() const
{
  double dispersion = 0;
  double averageProcessingTime = getAverageProcessingTime();
  for(auto processingTime: processingTimes_)
  {
    dispersion += pow(processingTime - averageProcessingTime, 2);
  }
  return dispersion / (processingTimes_.size() - 1);
}
