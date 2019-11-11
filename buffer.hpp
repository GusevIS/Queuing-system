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
  Request findNextRequest(std::vector<Request>::iterator first, std::vector<Request>::iterator last) const;
  void receiveRequest(Request request);
  bool isFull() const;
  bool isEmpty() const;
  int getSrcNumberOfOldestRequest();
  Request selectRequest();
  std::vector<Request> getRequests() const;
  void setBufferSize(unsigned int bufferSize);
  int getNumberOfRequests() const;

  unsigned int getBufferSize() const;

private:
  unsigned int bufferSize_;
  std::vector<Request> requests_;
};

#endif // BUFFER_H
