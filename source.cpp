#include "source.hpp"

const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

Source::Source(double lambda, int sourceNumber):
  lambda_(lambda),
  requestsCount_(0),
  sourceNumber_(sourceNumber),
  deniedRequestsCount_(0),
  processedRequestCount_(0)
{
  std::cout << sourceNumber << std::endl;
}

Request Source::generateRequest(double currentTime)
{
  double randNumber = 0.0;
  while (randNumber == 1.0 || randNumber == 0.0 )
    randNumber = rand() * fraction;
  double generationTime = currentTime + (-log(randNumber)/lambda_);

  Request request{sourceNumber_, generationTime};
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
