#ifndef SOURCE_H
#define SOURCE_H

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Request.h"

class Source
{
public:
  Source(double lambda, int sourceNumber);
  Request generateRequest();
  void increaseDeniedRequestsCount();
  int getDeniedRequestsCount() const;
  void setDeniedRequestsCount(int deniedRequestsCount);

private:
  double lambda_;
  int requestsCount_;
  double totalTime_;
  const int sourceNumber_;
  int deniedRequestsCount_;

};

#endif // SOURCE_H
