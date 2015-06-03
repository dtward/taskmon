#ifndef TASK_INFO_H
#define TASK_INFO_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>



class TaskInfo {
public: 
  // my enum type for properties, this will be used for sorting
  enum property {CPU, MEM};


  // the members
protected:
  // time will be used by the children class
  time_t _lastTime;
  time_t _thisTime;
  // the process
private:
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

  static property _sortBy;


public:


  // constructors
  // default constructor
  TaskInfo(void);
  // pid constructor
  TaskInfo(int pid);

  // get set
  int getPid() const;
  void setPid(int pid);
  bool getIsAlive() const;
  std::string getName() const;
  double getCpu() const;
  double getMem() const;
  double getIoIn() const;
  double getIoOut() const;
  static property getSortBy();
  static void setSortBy(property p);

  // update info
  void update();

  // print info
  void print() const;


  // overload comparison
  bool operator<(  TaskInfo const & t) const;
  bool operator>(  TaskInfo const & t) const;
};


#endif
