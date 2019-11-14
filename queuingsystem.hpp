#ifndef QUEUINGSYSTEM_H
#define QUEUINGSYSTEM_H

#include <QMainWindow>
#include <QValidator>

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <conio.h>
#include <windows.h>
#include "buffer.hpp"
#include "device.hpp"
#include "source.hpp"
#include "event.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const double MAX_DOUBLE_INPUT = 1000;
const double MIN_DOUBLE_INPUT = -1000;
const int MAX_INT_INPUT = 1000;
const int MIN_INT_INPUT = -1000;

class QueuingSystem: public QMainWindow
{
  Q_OBJECT

public:
  QueuingSystem(QWidget *parent = nullptr);
  ~QueuingSystem();
  void initializeSystem(int numberOfSources, int numberOfRequests,
                        int numberOfDevices, double lambda, unsigned int bufferSize,
                        int alpha, int beta);
  void startSystem();
  Event executeNextEvent();
  void showStepStates() const;

  Request chooseEarliestRequest(std::vector<Request> requestsToBuffer) const;
  Device chooseDevice(std::vector<Device> devices) const;
  EventType determineNextEvent() const;
  bool requiredReqNumberGenerated() const;
  void updateDevices();

  std::vector<Event> events() const;
  void setEvents(const std::vector<Event> &events);

  void calculateServiceCoefficient();

private slots:
  void on_autoSimulateBtn_clicked();
  void on_applyBtn_clicked();

  void on_enterStepBtn_clicked();

  void on_nextStepBtn_clicked();

  void on_prevStepBtn_clicked();

private:
  Ui::MainWindow *ui;
  QDoubleValidator doubleValidator_;
  QIntValidator intValidator_;
  bool systemIsActive_;
  std::vector<Source> sources_;
  std::vector<Device> devices_;
  std::vector<Request> generatedRequests_;
  Buffer buffer_;
  double currentTime_;
  int numberOfRequests_;
  std::vector<Event> events_;
  int currentStep_ = 0;
};

#endif // QUEUINGSYSTEM_H
