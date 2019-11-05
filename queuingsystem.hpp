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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum NextEvent
{
  SETTING_TO_BUFFER,
  SETTING_TO_DEVICE,
  FROM_SOURCE_TO_DEVICE
};

class QueuingSystem: public QMainWindow
{
  Q_OBJECT

public:
  QueuingSystem(QWidget *parent = nullptr);
  ~QueuingSystem();
  void initializeSystem(int numberOfSources, int numberOfRequests,
                        int numberOfDevices, double lambda, unsigned int bufferSize);
  void startSystem();
  Request chooseEarliestRequest(std::vector<Request> requestsToBuffer) const;
  Device chooseDevice(std::vector<Device> devices) const;
  NextEvent determineNextEvent() const;
  bool requiredReqNumberGenerated() const;

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
};

#endif // QUEUINGSYSTEM_H
