#include "queuingsystem.hpp"
#include "ui_mainwindow.h"

QueuingSystem::QueuingSystem(QWidget *parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  regExpValidator_(QRegExp("[+-]?\\d*[\\.,]?\\d+"), this),
  intValidator_(MIN_INT_INPUT, MAX_INT_INPUT, this),
  buffer_(0),
  currentTime_(0),
  numberOfRequests_(0)
{
  ui->setupUi(this);

  ui->lambdaInputLine->setValidator(&regExpValidator_);
  ui->bufferSizeInputLine->setValidator(&intValidator_);
  ui->numSourcesInputLine->setValidator(&intValidator_);
  ui->numDevicesInputLine->setValidator(&intValidator_);
  ui->numRequestInputLine->setValidator(&intValidator_);
  ui->alphaInputLine->setValidator(&regExpValidator_);
  ui->betaInputLine->setValidator(&regExpValidator_);

  ui->enterStepBtn->hide();

  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

QueuingSystem::~QueuingSystem()
{
  delete ui;
}

void QueuingSystem::initializeSystem(int numberOfSources, int numberOfRequests,
                      int numberOfDevices, double lambda, unsigned int bufferSize,
                      double alpha, double beta)
{
  buffer_.setBufferSize(bufferSize);
  currentTime_ = 0;
  numberOfRequests_ = numberOfRequests;
  sources_.clear();
  devices_.clear();
  events_.clear();

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
  ui->enterStepBtn->hide();
  double lambda = ui->lambdaInputLine->text().toDouble();

  unsigned int bufferSize = ui->bufferSizeInputLine->text().toUInt();
  int sourcesNumber = ui->numSourcesInputLine->text().toInt();
  int devicesNumber = ui->numDevicesInputLine->text().toInt();
  int requestsNumber = ui->numRequestInputLine->text().toInt();
  double alpha = ui->alphaInputLine->text().toDouble();
  double beta = ui->betaInputLine->text().toDouble();
  if((bufferSize != 0) && (sourcesNumber != 0) && (devicesNumber != 0) && (requestsNumber != 0) && (beta > alpha))
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
  ui->bufferStateTextBrowser->setText(QString::number(events_[currentStep_].getNumberOfRequestsInBuffer()) +
                                      QString::fromStdString("/" + std::to_string(buffer_.getBufferSize())));
  ui->stepCountTextBrowser->setText(QString::number(events_.size()));

  std::vector<DeviceStatus>deviceStatuses = events_[currentStep_].getDevicesIsBusy();
  ui->deviceStatusTableWidget->setRowCount(0);
  for(auto device: deviceStatuses){
    ui->deviceStatusTableWidget->insertRow(ui->deviceStatusTableWidget->rowCount());
    ui->deviceStatusTableWidget->setItem(ui->deviceStatusTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(device.deviceNumber)));
    ui->deviceStatusTableWidget->setItem(ui->deviceStatusTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(device.status)));
  }
  ui->deviceStatusTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->deviceStatusTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->changeLogTextBrowser->setText(QString::fromStdString(events_[currentStep_].getChangeLog()));

  std::vector<SourceStatus>sourceStatuses = events_[currentStep_].getSourcesStatuses();
  ui->sourceStatusTableWidget->setRowCount(0);
  for(auto source: sourceStatuses){
    ui->sourceStatusTableWidget->insertRow(ui->sourceStatusTableWidget->rowCount());
    ui->sourceStatusTableWidget->setItem(ui->sourceStatusTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(source.sourceNumber)));

    QString qString;
    if(source.isIdle)
      qString = "Idle";
    else
      qString = "generate Request";

    ui->sourceStatusTableWidget->setItem(ui->sourceStatusTableWidget->rowCount() - 1, 1, new QTableWidgetItem(qString));
    ui->sourceStatusTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->sourceStatusTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  }

  std::vector<BufferRequest> bufferStatus = events_[currentStep_].getBufferStatus();
  ui->bufferTableWidget->setRowCount(0);

  for(auto request: bufferStatus){
    ui->bufferTableWidget->insertRow(ui->bufferTableWidget->rowCount());
    ui->bufferTableWidget->setItem(ui->bufferTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(request.sourceNumber)));
    ui->bufferTableWidget->setItem(ui->bufferTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(request.arrivalTime)));
  }
  ui->bufferTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->bufferTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void QueuingSystem::on_autoSimulateBtn_clicked()
{
  startSystem();
  ui->autoSourceTableWidget->setRowCount(0);
  for(auto source: sources_){
    ResultSet resultSet{source.getSourceNumber(), (static_cast<double>(source.getDeniedRequestsCount())  / static_cast<double>(source.getRequestCount())),
                        source.getProcessedRequestsCount(), source.getDeniedRequestsCount(), source.getRequestCount(), source.getAverageWaitingTime(),
                        source.getAverageSystemTime(), source.getAverageProcessingTime(), source.getDispersionOfProcessingTime(), source.getDispersionOfWaitingTime()
                       };
    ui->autoSourceTableWidget->insertRow(ui->autoSourceTableWidget->rowCount());
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(resultSet.sourceNumber)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(resultSet.systemTime)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(resultSet.processedReqNumber)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(resultSet.failedRequestsNumber)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::number(resultSet.bufferTime)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 5, new QTableWidgetItem(QString::number(resultSet.processingTime)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 6, new QTableWidgetItem(QString::number(resultSet.probabilityOfFailure)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 7, new QTableWidgetItem(QString::number(resultSet.dispersionServTime)));
    ui->autoSourceTableWidget->setItem(ui->autoSourceTableWidget->rowCount() - 1, 8, new QTableWidgetItem(QString::number(resultSet.dispersionWaitTime)));
  }
  ui->autoSourceTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  calculateServiceCoefficient();
  ui->autoDeviceTableWidget->setRowCount(0);
  for(auto device: devices_){
    ui->autoDeviceTableWidget->insertRow(ui->autoDeviceTableWidget->rowCount());
    ui->autoDeviceTableWidget->setItem(ui->autoDeviceTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(device.getDeviceNumber())));
    ui->autoDeviceTableWidget->setItem(ui->autoDeviceTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(device.getServiceCoefficient())));
  }
  ui->autoDeviceTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  ui->enterStepBtn->show();
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

void QueuingSystem::calculateServiceCoefficient()
{
  std::vector<double> serviceCoefficients;
  auto lastFreeDevice = std::max_element(devices_.begin(), devices_.end(), [](const Device &dev1, const Device &dev2) {
    return (dev1.getReleaseTime() < dev2.getReleaseTime());
  });
  for(auto &device: devices_)
  {
    device.setServiceCoefficient(device.getTotalServiceTime() / (*lastFreeDevice).getReleaseTime());
  }
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
  if((device.getReleaseTime() <= earliestRequest.getGenerationTime()) && (!buffer_.isEmpty()))
  {
    return EventType::SETTING_TO_DEVICE;
  }
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
    Request selectedRequest = buffer_.selectRequest();
    if(device.getReleaseTime() > selectedRequest.getBufferArriveTime())
      currentTime_ = devices_[device.getDeviceNumber()].getReleaseTime();
    else
      currentTime_ = selectedRequest.getBufferArriveTime();

    sources_[selectedRequest.getSourceNumber()].increaseProcessedRequestCount();
    selectedRequest.calculateWaitingTime(currentTime_);
    sources_[selectedRequest.getSourceNumber()].addBufferTime(selectedRequest.getWaitingTime());

    double serviceTime = devices_[device.getDeviceNumber()].calculateServiceTime(currentTime_, selectedRequest);
    sources_[selectedRequest.getSourceNumber()].addProcessingTime(serviceTime);

    std::string sourceNumber = std::to_string(selectedRequest.getSourceNumber());
    std::string generationTime = std::to_string(selectedRequest.getGenerationTime());
    std::string deviceNumber = std::to_string(device.getDeviceNumber());
    changeLog += "Request from source " + sourceNumber + " " + generationTime + " sent from BUFFER to" + " DEVICE " + deviceNumber;
    break;
  }
  }

  updateDevices();

  std::vector<DeviceStatus> devicesStatus;
  for(auto device: devices_){
    devicesStatus.push_back( {device.getDeviceNumber(), device.getStatus()} );
  }

  std::vector<BufferRequest> bufferStatus;
  for(auto &request: buffer_.getRequests()){
    bufferStatus.push_back( {request.getSourceNumber(), request.getBufferArriveTime()} );
  }

  Event newEvent(eventType, currentTime_, changeLog, buffer_.getRequests().size(), devicesStatus, sourceStatuses, bufferStatus);
  return newEvent;
}
