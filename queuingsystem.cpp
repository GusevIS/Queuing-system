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
                      int numberOfDevices, double lambda, unsigned int bufferSize,
                      int alpha, int beta)
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
    Device device(i, alpha, beta);
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
  int alpha = ui->alphaInputLine->text().toInt();
  int beta = ui->betaInputLine->text().toInt();
  initializeSystem(sourcesNumber, requestsNumber, devicesNumber, lambda, bufferSize, alpha, beta);
}

std::vector<Event> QueuingSystem::events() const
{
    return events_;
}

void QueuingSystem::setEvents(const std::vector<Event> &events)
{
    events_ = events;
}

void QueuingSystem::showStepStates() const
{
  ui->timeTextBrowser->setText(QString::number(events_[currentStep_].getSystemTime()));
  ui->bufferStateTextBrowser->setText(QString::number(events_[currentStep_].getNumberOfRequestsInBuffer()) + QString::fromStdString("/" + std::to_string(buffer_.getBufferSize())));
  ui->stepCountTextBrowser->setText(QString::number(events_.size()));

  std::vector<DeviceStatus>deviceStatuses = events_[currentStep_].getDevicesIsBusy();
  ui->deviceStatusTableWidget->setRowCount(0);
  for(auto device: deviceStatuses){
    ui->deviceStatusTableWidget->insertRow(ui->deviceStatusTableWidget->rowCount());
    ui->deviceStatusTableWidget->setItem(ui->deviceStatusTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(device.deviceNumber)));
    ui->deviceStatusTableWidget->setItem(ui->deviceStatusTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(device.status)));
  }
  ui->deviceStatusTableWidget->horizontalHeader()->setSectionResizeMode(1 , QHeaderView::Stretch);
  ui->changeLogTextBrowser->setText(QString::fromStdString(events_[currentStep_].getChangeLog()));

  std::vector<SourceStatus>sourceStatuses = events_[currentStep_].getSourcesStatuses();
  ui->sourceStatusTableWidget->setRowCount(0);
  for(auto source: sourceStatuses){
    ui->sourceStatusTableWidget->insertRow(ui->sourceStatusTableWidget->rowCount());
    ui->sourceStatusTableWidget->setItem(ui->sourceStatusTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(source.sourceNumber )));

    QString qString;
    if(source.isIdle)
      qString = "Idle";
    else
      qString = "generate Request";

    ui->sourceStatusTableWidget->setItem(ui->sourceStatusTableWidget->rowCount() - 1, 1, new QTableWidgetItem(qString));
  }
}

void QueuingSystem::on_autoSimulateBtn_clicked()
{
  startSystem();
  ui->tableWidget->setRowCount(0);
  for(auto source: sources_){
    ResultSet resultSet{source.getSourceNumber(), (static_cast<double>(source.getDeniedRequestsCount())  / static_cast<double>(source.getRequestCount())),
                        source.getProcessedRequestsCount(), source.getDeniedRequestsCount(), source.getRequestCount(), (source.getBufferTime() / source.getRequestCount()),
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

void QueuingSystem::on_enterStepBtn_clicked()
{
  int stepNumber = ui->steplineEdit->text().toInt();
  currentStep_ = stepNumber;

  showStepStates();
}

void QueuingSystem::on_nextStepBtn_clicked()
{
  if(currentStep_ < events_.size())
    currentStep_++;
  ui->steplineEdit->setText(QString::number(currentStep_));
  showStepStates();
}

void QueuingSystem::on_prevStepBtn_clicked()
{
  if(currentStep_ > 0)
    currentStep_--;
  ui->steplineEdit->setText(QString::number(currentStep_));
  showStepStates();
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

void QueuingSystem::updateDevices()
{
  for(auto &device: devices_)
  {
    device.updateStatus(currentTime_);
  }
}

void QueuingSystem::startSystem()
{
  for(auto &source: sources_)
  {
    Request newRequest = source.generateRequest(currentTime_);
    generatedRequests_.push_back(newRequest);
  }

  while(!(buffer_.isEmpty() && generatedRequests_.empty())){
    Event event = executeNextEvent();
    events_.push_back(event);
  }
}

Event QueuingSystem::executeNextEvent()
{
  EventType eventType = determineNextEvent();
  std::string changeLog;
  std::vector<SourceStatus> sourceStatuses;
  for(auto source: sources_)
  {
    sourceStatuses.push_back( {source.getSourceNumber(), true} );
  }

  switch (eventType)
  {
  case EventType::SETTING_TO_BUFFER:
  {
    Request earliestRequest = chooseEarliestRequest(generatedRequests_);
    currentTime_ = earliestRequest.getGenerationTime();
    if(buffer_.isFull()){
      std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
      std::string generationTime = std::to_string(earliestRequest.getGenerationTime());

      Request failedRequest = buffer_.getFailedRequest();
      failedRequest.calculateWaitingTime(currentTime_);
      sources_[failedRequest.getSourceNumber()].addBufferTime(failedRequest.getWaitingTime());
      changeLog += "Request from source " + sourceNumber + " " + generationTime + " failed;   ";

      sources_[buffer_.getSrcNumberOfOldestRequest()].increaseDeniedRequestsCount();
    }
    generatedRequests_.erase(std::remove(generatedRequests_.begin(), generatedRequests_.end(), earliestRequest));
    earliestRequest.setBufferArriveTime(currentTime_);
    buffer_.receiveRequest(earliestRequest);
    if(sources_[earliestRequest.getSourceNumber()].getRequestCount() < numberOfRequests_){
      Request newRequest = sources_[earliestRequest.getSourceNumber()].generateRequest(currentTime_);
      generatedRequests_.push_back(newRequest);
      sourceStatuses[earliestRequest.getSourceNumber()].isIdle = false;
    }

    std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
    std::string generationTime = std::to_string(earliestRequest.getGenerationTime());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + " sent to" + " BUFFER";
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

    double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_, selectedRequest);
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

    double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_, earliestRequest);
    sources_[earliestRequest.getSourceNumber()].increaseProcessedRequestCount();
    sources_[earliestRequest.getSourceNumber()].addProcessingTime(serviceTime);

    if(sources_[earliestRequest.getSourceNumber()].getRequestCount() < numberOfRequests_){
      Request newRequest = sources_[earliestRequest.getSourceNumber()].generateRequest(currentTime_);
      generatedRequests_.push_back(newRequest);
      sourceStatuses[earliestRequest.getSourceNumber()].isIdle = false;
    }
    std::string sourceNumber = std::to_string(earliestRequest.getSourceNumber());
    std::string generationTime = std::to_string(earliestRequest.getGenerationTime());
    std::string deviceNumber = std::to_string(device.getDeviceNumber());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + " sent from SOURCE to " + " DEVICE " + deviceNumber;
    break;
  }
  }

  updateDevices();

  std::vector<DeviceStatus> devicesStatus;
  for(auto device: devices_){
    devicesStatus.push_back( {device.getDeviceNumber(), device.getStatus()} );
  }
  Event newEvent(eventType, currentTime_, changeLog, buffer_.getRequests().size(), devicesStatus, sourceStatuses);

/*
  for(auto device: devices_){
    std::cout << "device " << device.getDeviceNumber() << "-" << device.getStatus() << std::endl;
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
  */

  return newEvent;
}
