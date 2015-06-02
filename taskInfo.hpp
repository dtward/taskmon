#ifndef TASK_INFO_H
#define TASK_INFO_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

class TaskInfo {
private:
  // the members
  time_t _lastTime;
  time_t _thisTime;
  // the process
  int _pid;
  bool _isAlive;
  // the things I'm interested in
  std::string _name;
  double _cpu;
  double _cpu0; // because I need to measure a difference over a specific time interval
  double _mem;
  double _ioIn;
  double _ioIn0;
  double _ioOut;
  double _ioOut0;

  // this method will be called by the constructor
  // it will be similarto update()
  // but update will take rate of change
  void update0();



public:
  // constructors
  // default constructor
  TaskInfo(void);
  // pid constructor
  TaskInfo(int pid);

  // get set
  int getPid();
  void setPid(int pid);
  bool getIsAlive();
  std::string getName();
  double getCpu();
  double getMem();
  double getIoIn();
  double getIoOut();

  // update info
  void update();

  // print info
  void print();
};


#endif
