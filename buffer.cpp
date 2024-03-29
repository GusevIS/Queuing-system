#include "buffer.hpp"

Buffer::Buffer(unsigned int bufferSize):
  bufferSize_(bufferSize)
{

}

int Buffer::getSrcNumberOfOldestRequest()
{
  return requests_.front().getSourceNumber();
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
  return requests_.empty();
}

Request Buffer::selectRequest()
{
  Request selectedRequest = requests_.back();
  requests_.pop_back();
  return selectedRequest;
}

Request Buffer::getFailedRequest()
{
  return requests_.front();
}

int Buffer::getNumberOfRequests() const
{
  return requests_.size();
}

unsigned int Buffer::getBufferSize() const
{
  return bufferSize_;
}

std::vector<Request> Buffer::getRequests() const
{
  return requests_;
}

void Buffer::setBufferSize(unsigned int bufferSize)
{
  bufferSize_ = bufferSize;
}
