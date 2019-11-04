#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_applyBtn_clicked()
{
  int lambda = ui->lambdaInputLine->text().toInt();
  unsigned int bufferSize = ui->bufferSizeInputLine->text().toUInt();
  int sourcesNumber = ui->numSourcesInputLine->text().toInt();
  int devicesNumber = ui->numDevicesInputLine->text().toInt();
  int requestsNumber = ui->numRequestInputLine->text().toInt();
  QueuingSystem queuingSystem(sourcesNumber, requestsNumber, devicesNumber, lambda, bufferSize);
  queuingSystem_ = &queuingSystem;
  queuingSystem_->startSystem();
}

void MainWindow::on_autoSimulateBtn_clicked()
{
  //queuingSystem_->startSystem();
}
