#ifndef QUEUINGSYSTEM_H
#define QUEUINGSYSTEM_H

#include <QMainWindow>
#include <vector>
#include <iostream>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include "buffer.hpp"
#include "device.hpp"
#include "source.hpp"
#include "event.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QueuingSystem: public QMainWindow
{
  Q_OBJECT

public:
  QueuingSystem(QWidget *parent = nullptr);
  ~QueuingSystem();
  void initializeSystem(int numberOfSources, int numberOfRequests,
                        int numberOfDevices, double lambda, unsigned int bufferSize);
  void startSystem();
  Event executeNextEvent();

  Request chooseEarliestRequest(std::vector<Request> requestsToBuffer) const;
  Device chooseDevice(std::vector<Device> devices) const;
  EventType determineNextEvent() const;
  bool requiredReqNumberGenerated() const;

  std::list<Event> events() const;
  void setEvents(const std::list<Event> &events);

private slots:
  void on_autoSimulateBtn_clicked();
  void on_applyBtn_clicked();

private:
  Ui::MainWindow *ui;
  bool systemIsActive_;
  std::vector<Source> sources_;
  std::vector<Device> devices_;
  std::vector<Request> generatedRequests_;
  Buffer buffer_;
  double currentTime_;
  int numberOfRequests_;
  std::list<Event> events_;
};

#endif // QUEUINGSYSTEM_H
