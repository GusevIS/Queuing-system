#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "queuingsystem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_autoSimulateBtn_clicked();
    void on_applyBtn_clicked();

private:
    Ui::MainWindow *ui;
    QueuingSystem *queuingSystem_;
};
#endif // MAINWINDOW_H
