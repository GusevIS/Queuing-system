#include <QApplication>
#include <ctime>
#include "queuingsystem.hpp"

int main(int argc, char *argv[])
{
  srand(static_cast<unsigned int>(time(0)));

  QApplication a(argc, argv);
  QueuingSystem qSystem;
  qSystem.show();

  return a.exec();
}
