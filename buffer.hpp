#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <iostream>
#include "algorithm"
#include "Request.h"

class Buffer
{
public:
  Buffer(unsigned int bufferSize);
  void receiveRequest(Request request);
  bool isFull() const;
  bool isEmpty() const;
  int getSrcNumberOfOldestRequest();
  Request selectRequest();
  std::vector<Request> getRequests() const;
  void setBufferSize(unsigned int bufferSize);

private:
  unsigned int bufferSize_;
  std::vector<Request> requests_;
};

#endif // BUFFER_H
