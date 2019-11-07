#include "queuingsystem.hpp"
#include "ui_mainwindow.h"

QueuingSystem::QueuingSystem(QWidget *parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  buffer_(0),
  currentTime_(0),
  numberOfRequests_(0)
{
    ui->setupUi(this);
}

QueuingSystem::~QueuingSystem()
{
    delete ui;
}

void QueuingSystem::initializeSystem(int numberOfSources, int numberOfRequests,
                      int numberOfDevices, double lambda, unsigned int bufferSize)
{
  buffer_ = bufferSize;
  currentTime_ = 0;
  numberOfRequests_ = numberOfRequests;
  systemIsActive_ = true;

  for (int i = 0; i < numberOfSources; i++){
    Source source(lambda, i);
    sources_.push_back(source);
  }

  for (int i = 0; i < numberOfDevices; i++){
    Device device(i);
    devices_.push_back(device);
  }
}

bool QueuingSystem::requiredReqNumberGenerated() const
{
  for(auto source: sources_){
    if(source.getRequestCount() < numberOfRequests_)
      return false;
  }
  return true;
}

void QueuingSystem::on_applyBtn_clicked()
{
  int lambda = ui->lambdaInputLine->text().toInt();
  unsigned int bufferSize = ui->bufferSizeInputLine->text().toUInt();
  int sourcesNumber = ui->numSourcesInputLine->text().toInt();
  int devicesNumber = ui->numDevicesInputLine->text().toInt();
  int requestsNumber = ui->numRequestInputLine->text().toInt();
  initializeSystem(sourcesNumber, requestsNumber, devicesNumber, lambda, bufferSize);
}

std::list<Event> QueuingSystem::events() const
{
    return events_;
}

void QueuingSystem::setEvents(const std::list<Event> &events)
{
    events_ = events;
}

void QueuingSystem::on_autoSimulateBtn_clicked()
{
    startSystem();
    for(auto source: sources_){
        ResultSet resultSet{source.getSourceNumber(), (static_cast<double>(source.getDeniedRequestsCount())  / static_cast<double>(source.getRequestCount())),
                       source.getProcessedRequestsCount(), source.getDeniedRequestsCount(), source.getRequestCount(), (source.getBufferTime() / source.getProcessedRequestsCount()),
                       (source.getBufferTime() + source.getProcessingTime()) / source.getProcessedRequestsCount(), source.getProcessingTime() / source.getProcessedRequestsCount()};
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(resultSet.sourceNumber)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(resultSet.generateReqNumber)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(resultSet.processedReqNumber)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(resultSet.failedRequestsNumber)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(resultSet.bufferTime)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(resultSet.processingTime)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(resultSet.probabilityOfFailure)));
  }
}

bool operator== (const Request &request1, const Request &request2)
{
    return (request1.getGenerationTime() == request2.getGenerationTime() &&
            request1.getSourceNumber() == request2.getSourceNumber());
}

bool operator< (const Request &request1, const Request &request2)
{
    return request1.getGenerationTime() < request2.getGenerationTime();
}

bool operator< (const Device &device1, const Device &device2)
{
    return device1.getReleaseTime() < device2.getReleaseTime();
}

Request QueuingSystem::chooseEarliestRequest(std::vector<Request> generatedRequests_) const
{
  std::vector<Request>::iterator earliestRequest = std::min_element(generatedRequests_.begin(), generatedRequests_.end());
  return *earliestRequest;
}

Device QueuingSystem::chooseDevice(std::vector<Device> devices) const
{
  std::vector<Device>::iterator device = std::min_element(devices.begin(), devices.end());
  return *device;
}

EventType QueuingSystem::determineNextEvent() const
{
  if(generatedRequests_.empty())
    return EventType::SETTING_TO_DEVICE;
  Request earliestRequest = chooseEarliestRequest(generatedRequests_);
  Device device = chooseDevice(devices_);
  if(device.getReleaseTime() <= earliestRequest.getGenerationTime())
  {
    if(!buffer_.isEmpty())
      return EventType::SETTING_TO_DEVICE;
    else
      return EventType::FROM_SOURCE_TO_DEVICE;
  }
  else
    return EventType::SETTING_TO_BUFFER;
}

void QueuingSystem::startSystem()
{
  for(auto &source: sources_)
  {
    Request newRequest = source.generateRequest(currentTime_);
    generatedRequests_.push_back(newRequest);                               //generate first requests from sources
    std::cout << newRequest.getSourceNumber() << " " << newRequest.getGenerationTime() << "\n";
  }

  std::cout << "\n";

  while(true){
    Event event = executeNextEvent();
    events_.push_back(event);
  }

  while (!(buffer_.isEmpty() && generatedRequests_.empty()))
  {

//    Request earliestRequest = chooseEarliestRequest(generatedRequests_);
//    Device device = chooseDevice(devices_);
    NextEvent nextEvent = determineNextEvent();
    std::cout << nextEvent << std::endl;

    switch(nextEvent)
    {
      case NextEvent::SETTING_TO_BUFFER:
      {



        std::cout << "Request from source " << earliestRequest.getSourceNumber() << " " << earliestRequest.getGenerationTime() << "-->>" << " buffer" << std::endl;


      break;
      }
      case NextEvent::SETTING_TO_DEVICE:
      {

      }
      case NextEvent::FROM_SOURCE_TO_DEVICE:
      {
        Request earliestRequest = chooseEarliestRequest(generatedRequests_);
        Device device = chooseDevice(devices_);
        currentTime_ = earliestRequest.getGenerationTime();
        generatedRequests_.erase(std::remove(generatedRequests_.begin(), generatedRequests_.end(), earliestRequest));

        std::cout << "Request from source " << earliestRequest.getSourceNumber() << " " << earliestRequest.getGenerationTime() << "-->>" << device.getDeviceNumber() << std::endl;

        double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_);
        sources_[earliestRequest.getSourceNumber()].increaseProcessedRequestCount();
        sources_[earliestRequest.getSourceNumber()].addProcessingTime(serviceTime);

        if(sources_[earliestRequest.getSourceNumber()].getRequestCount() < numberOfRequests_){
          Request newRequest = sources_[earliestRequest.getSourceNumber()].generateRequest(currentTime_);
          generatedRequests_.push_back(newRequest);
        }
        break;
      }
    }
    std::cout << buffer_.isEmpty() << generatedRequests_.empty() << std::endl;
    /////////////////////////////////////////////////////////////////////////////////
        //Sleep(1000);
        for(auto device: devices_){
            std::cout << "device " << device.getDeviceNumber() << "-" << device.getReleaseTime() << std::endl;
        }
        std::cout << "requests left " << currentTime_ << std::endl;
        for(auto request: generatedRequests_){
            std::cout << request.getSourceNumber() << " " << request.getGenerationTime() << std::endl;
        }
        std::cout << "requests in buffer: " << std::endl;
        for(auto request: buffer_.getRequests())
        {
          std::cout << request.getSourceNumber() << " " << request.getGenerationTime() << "\n";
        }
        std::cout << "\n";
        for(auto source: sources_)
        {
          std::cout << "source " << source.getSourceNumber() << " " << source.getRequestCount() << " " << source.getDeniedRequestsCount() << " " << source.getProcessedRequestsCount() << "\n";
        }
        std::cout << "-----------------" << std::endl;

    /////////////////////////////////////////////////////////////////////////////////
  }
}

Event QueuingSystem::executeNextEvent()
{
  EventType eventType = determineNextEvent();
  std::string changeLog;

  switch (eventType)
  {
  case EventType::SETTING_TO_BUFFER:
  {
    Request earliestRequest = chooseEarliestRequest(generatedRequests_);
    currentTime_ = earliestRequest.getGenerationTime();
    if(buffer_.isFull()){
      std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
      std::string generationTime = std::to_string(earliestRequest.getGenerationTime());
      changeLog += "Request from source " + sourceNumber + " " + generationTime + " failed";

      sources_[buffer_.getSrcNumberOfOldestRequest()].increaseDeniedRequestsCount();
    }
    generatedRequests_.erase(std::remove(generatedRequests_.begin(), generatedRequests_.end(), earliestRequest));
    earliestRequest.setBufferArriveTime(currentTime_);
    buffer_.receiveRequest(earliestRequest);
    if(sources_[earliestRequest.getSourceNumber()].getRequestCount() < numberOfRequests_){
      Request newRequest = sources_[earliestRequest.getSourceNumber()].generateRequest(currentTime_);
      generatedRequests_.push_back(newRequest);
    }

    std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
    std::string generationTime = std::to_string(earliestRequest.getGenerationTime());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + "sent to" + " BUFFER";
    break;
  }
  case EventType::SETTING_TO_DEVICE:
  {
    Device device = chooseDevice(devices_);
    currentTime_ = devices_[device.getDeviceNumber()].getReleaseTime();
    Request selectedRequest = buffer_.selectRequest();

    sources_[selectedRequest.getSourceNumber()].increaseProcessedRequestCount();
    selectedRequest.calculateWaitingTime(currentTime_);
    sources_[selectedRequest.getSourceNumber()].addBufferTime(selectedRequest.getWaitingTime());

    double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_);
    sources_[selectedRequest.getSourceNumber()].addProcessingTime(serviceTime);

    std::cout << "Request from source " << selectedRequest.getSourceNumber() << " " << selectedRequest.getGenerationTime() << "-->>" << device.getDeviceNumber() << std::endl;
    std::string sourceNumber = std::to_string(selectedRequest.getSourceNumber());
    std::string generationTime = std::to_string(selectedRequest.getGenerationTime());
    std::string deviceNumber = std::to_string(device.getDeviceNumber());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + " sent from BUFFER to" + " DEVICE " + deviceNumber;
    break;
  }
  case EventType::FROM_SOURCE_TO_DEVICE:
  {
    Request earliestRequest = chooseEarliestRequest(generatedRequests_);
    Device device = chooseDevice(devices_);
    currentTime_ = earliestRequest.getGenerationTime();
    generatedRequests_.erase(std::remove(generatedRequests_.begin(), generatedRequests_.end(), earliestRequest));

    std::cout << "Request from source " << earliestRequest.getSourceNumber() << " " << earliestRequest.getGenerationTime() << "-->>" << device.getDeviceNumber() << std::endl;

    double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_);
    sources_[earliestRequest.getSourceNumber()].increaseProcessedRequestCount();
    sources_[earliestRequest.getSourceNumber()].addProcessingTime(serviceTime);

    if(sources_[earliestRequest.getSourceNumber()].getRequestCount() < numberOfRequests_){
      Request newRequest = sources_[earliestRequest.getSourceNumber()].generateRequest(currentTime_);
      generatedRequests_.push_back(newRequest);
    }
    std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
    std::string generationTime = std::to_string(earliestRequest.getGenerationTime());
    std::string deviceNumber = std::to_string(device.getDeviceNumber());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + " sent from SOURCE to " + " DEVICE " + deviceNumber;
    break;
  }
  }

  std::vector<bool> devicesStatus;
  for(auto device: devices_){
    devicesStatus.push_back(!(device.getReleaseTime() == currentTime_));
  }
  Event newEvent(eventType, currentTime_, changeLog, buffer_.getRequests().size(), devicesStatus);
}
