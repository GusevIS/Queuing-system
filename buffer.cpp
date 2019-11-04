#define _DEBUG

#include "buffer.hpp"

Buffer::Buffer(unsigned int bufferSize):
  bufferSize_(bufferSize)
{

}

int Buffer::getSrcNumberOfOldestRequest()
{
  return requests_.front().sourceNumber;
}

void Buffer::receiveRequest(Request request)
{
  if(requests_.size() < bufferSize_){
    requests_.push_back(request);
  }else{
    requests_.erase(requests_.begin());
    requests_.push_back(request);
  }
}

bool Buffer::isFull() const
{
  return (requests_.size() >= bufferSize_);
}

bool Buffer::isEmpty() const
{
  return (requests_.empty());
}

Request Buffer::selectRequest()
{
    std::vector<Request>::iterator selectedRequest = std::min_element(requests_.begin(), requests_.end(),
                                                                      [](const Request &req1, const Request &req2){
      return req1.sourceNumber < req2.sourceNumber;
    });
    requests_.erase(std::remove_if(requests_.begin(), requests_.end(), [selectedRequest](Request &req1){
      return (req1.sourceNumber == selectedRequest->sourceNumber) && (req1.generationTime == selectedRequest->generationTime);
    }));
  return *selectedRequest;
}

std::vector<Request> Buffer::getRequests() const
{
  return requests_;
}
