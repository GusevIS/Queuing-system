#include "source.hpp"

const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

Source::Source(double lambda, int sourceNumber):
    lambda_(lambda),
    requestsCount_(0),
    totalTime_(0),
    sourceNumber_(sourceNumber)
{
  std::cout << sourceNumber << std::endl;
}

Request Source::generateRequest()
{
  double randomNumber = 0.0;
  while (randomNumber == 1.0 || randomNumber == 0.0 )
    randomNumber = rand() * fraction;
  totalTime_ += (-log(randomNumber)/lambda_);

  Request request{sourceNumber_, totalTime_};
  requestsCount_++;

  return request;
//  buffer_->receiveRequest(request);

}

void Source::increaseDeniedRequestsCount(){
  deniedRequestsCount_++;
}

int Source::getDeniedRequestsCount() const{
  return deniedRequestsCount_;
}

void Source::setDeniedRequestsCount(int deniedRequestsCount){
    deniedRequestsCount_ = deniedRequestsCount;
}
