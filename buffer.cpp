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

#ifdef _DEBUG
    std::cout << "requests in buffer: " << std::endl;
    for(auto &request: requests_)
    {
      std::cout << request.sourceNumber << " " << request.generationTime << "\n";
    }
    std::cout << "\n";
#endif
}

bool Buffer::isFull() const
{
  return (requests_.size() >= bufferSize_);
}
