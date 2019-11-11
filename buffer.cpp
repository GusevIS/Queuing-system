#include "buffer.hpp"

Buffer::Buffer(unsigned int bufferSize):
  bufferSize_(bufferSize)
{

}

Request Buffer::findNextRequest(std::vector<Request>::iterator first, std::vector<Request>::iterator last) const
{
  if (first == last)
    return *last;
  std::vector<Request>::iterator smallest = first;
  ++first;
  for (; first != last; ++first) {
    if ((*first).getSourceNumber() <= (*smallest).getSourceNumber())
      smallest = first;
  }
  return *smallest;
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
    Request selectedRequest = findNextRequest(requests_.begin(), requests_.end());
    requests_.erase(std::remove_if(requests_.begin(), requests_.end(), [selectedRequest](Request &req1){
      return (req1.getSourceNumber() == selectedRequest.getSourceNumber()) && (req1.getGenerationTime() == selectedRequest.getGenerationTime());
    }));
  return selectedRequest;
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
