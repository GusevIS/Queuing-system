#include "queuingsystem.hpp"

QueuingSystem::QueuingSystem(int numberOfSources, int workIntensity,
                             int numberOfDevices, int serviceIntensity, unsigned int bufferSize):
  buffer_(bufferSize)
{

  double lambda = 2; //temp

  for (int i = 0; i < numberOfSources; i++){
    Source source(lambda, i);
    sources_.push_back(source);
  }

  systemIsActive = true;
}

bool operator== (const Request &request1, const Request &request2)
{
    return (request1.generationTime == request2.generationTime &&
            request1.sourceNumber == request2.sourceNumber);
}

bool operator< (const Request &request1, const Request &request2)
{
    return request1.generationTime < request2.generationTime;
}

Request QueuingSystem::chooseEarliestRequest(std::vector<Request> requestsToBuffer)
{
  std::vector<Request>::iterator earliestRequest = std::min_element(requestsToBuffer.begin(), requestsToBuffer.end());
  return *earliestRequest;
}

void QueuingSystem::startSystem()
{
  std::vector<Request> requestsToBuffer;
  for(auto &source: sources_)
  {
    Request newRequest = source.generateRequest();
    requestsToBuffer.push_back(newRequest);  //generate first requests from sources
    std::cout << newRequest.sourceNumber << " " << newRequest.generationTime << "\n";
  }

  std::cout << "\n";

  while(systemIsActive)
  {
    Request earliestRequest = chooseEarliestRequest(requestsToBuffer);
    if(buffer_.isFull()){
      sources_[buffer_.getSrcNumberOfOldestRequest()].increaseDeniedRequestsCount();
      //systemIsActive = false;
    }
    std::remove(requestsToBuffer.begin(), requestsToBuffer.end(), earliestRequest);
    buffer_.receiveRequest(earliestRequest);
    Sleep(3000);
  }
}
