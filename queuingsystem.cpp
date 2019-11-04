#include "queuingsystem.hpp"

QueuingSystem::QueuingSystem(int numberOfSources, int workIntensity,
                             int numberOfDevices, int serviceIntensity, unsigned int bufferSize):
  buffer_(bufferSize),
  currentTime_(0)
{

  double lambda = 2; //temp

  for (int i = 0; i < numberOfSources; i++){
    Source source(lambda, i);
    sources_.push_back(source);
  }

  for (int i = 0; i < numberOfDevices; i++){
    Device device(i);
    devices_.push_back(device);
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

bool operator< (const Device &device1, const Device &device2)
{
    return device1.getReleaseTime() < device2.getReleaseTime();
}

Request QueuingSystem::chooseEarliestRequest(std::vector<Request> requestsToBuffer) const
{
  std::vector<Request>::iterator earliestRequest = std::min_element(requestsToBuffer.begin(), requestsToBuffer.end());
  return *earliestRequest;
}

Device QueuingSystem::chooseDevice(std::vector<Device> devices) const
{
  std::vector<Device>::iterator device = std::min_element(devices.begin(), devices.end());
  return *device;
}

NextEvent QueuingSystem::determineNextEvent(Request earliestRequest, Device device) const
{
  if(device.getReleaseTime() <= earliestRequest.generationTime)
  {
    if(!buffer_.isEmpty())
      return NextEvent::SETTING_TO_DEVICE;
    else
      return NextEvent::FROM_SOURCE_TO_DEVICE;
  }
  else
    return NextEvent::SETTING_TO_BUFFER;
}

void QueuingSystem::startSystem()
{
  std::vector<Request> requestsToBuffer;
  for(auto &source: sources_)
  {
    Request newRequest = source.generateRequest(currentTime_);
    requestsToBuffer.push_back(newRequest);                               //generate first requests from sources
    std::cout << newRequest.sourceNumber << " " << newRequest.generationTime << "\n";
  }

  std::cout << "\n";

  while(systemIsActive)
  {
    Request earliestRequest = chooseEarliestRequest(requestsToBuffer);
    Device device = chooseDevice(devices_);
    NextEvent nextEvent = determineNextEvent(earliestRequest, device);
    std::cout << nextEvent << std::endl;

    switch(nextEvent)
    {
      case NextEvent::SETTING_TO_BUFFER:
      {
        if(buffer_.isFull()){
          sources_[buffer_.getSrcNumberOfOldestRequest()].increaseDeniedRequestsCount();
        }
        requestsToBuffer.erase(std::remove(requestsToBuffer.begin(), requestsToBuffer.end(), earliestRequest));
        currentTime_ = earliestRequest.generationTime;
        buffer_.receiveRequest(earliestRequest);
        std::cout << "Request from source " << earliestRequest.sourceNumber << " " << earliestRequest.generationTime << "-->>" << " buffer" << std::endl;
        Request newRequest = sources_[earliestRequest.sourceNumber].generateRequest(currentTime_);
        requestsToBuffer.push_back(newRequest);
      break;
      }
      case NextEvent::SETTING_TO_DEVICE:
      {
        currentTime_ = devices_[device.getDeviceNumber()].getReleaseTime();
        Request selectedRequest = buffer_.selectRequest();
        double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_);
        sources_[selectedRequest.sourceNumber].increaseProcessedRequestCount();
        std::cout << "Request from buffer " << earliestRequest.sourceNumber << " " << earliestRequest.generationTime << "-->>" << device.getDeviceNumber() << std::endl;
        break;
      }
      case NextEvent::FROM_SOURCE_TO_DEVICE:
      {
        currentTime_ = earliestRequest.generationTime;
        requestsToBuffer.erase(std::remove(requestsToBuffer.begin(), requestsToBuffer.end(), earliestRequest));

        std::cout << "Request from source " << earliestRequest.sourceNumber << " " << earliestRequest.generationTime << "-->>" << device.getDeviceNumber() << std::endl;

        double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_);
        sources_[earliestRequest.sourceNumber].increaseProcessedRequestCount();
        Request newRequest = sources_[earliestRequest.sourceNumber].generateRequest(currentTime_);
        requestsToBuffer.push_back(newRequest);
        break;
      }
    }
    /////////////////////////////////////////////////////////////////////////////////
        Sleep(1000);
        for(auto device: devices_){
            std::cout << "device " << device.getDeviceNumber() << "-" << device.getReleaseTime() << std::endl;
        }
        std::cout << "requests left " << currentTime_ << std::endl;
        for(auto request: requestsToBuffer){
            std::cout << request.sourceNumber << " " << request.generationTime << std::endl;
        }
        std::cout << "requests in buffer: " << std::endl;
        for(auto request: buffer_.getRequests())
        {
          std::cout << request.sourceNumber << " " << request.generationTime << "\n";
        }
        std::cout << "\n";
        for(auto source: sources_)
        {
          std::cout << "source " << source.getSourceNumber() << " " << source.getDeniedRequestsCount() << " " << source.getProcessedRequestsCount() << "\n";
        }
        std::cout << "-----------------" << std::endl;

    /////////////////////////////////////////////////////////////////////////////////
  }
}
