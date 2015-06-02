#include "taskInfo.hpp"

// I need to wait some time and measure twice
// think about how to do this
// probably get the initial values in the constructor
// then update is a different function

// constructor
TaskInfo::TaskInfo(){
  _isAlive = false; // do nothing
}
TaskInfo::TaskInfo(int pid){
  _pid = pid;
  update0(); // first time only, get things that need to be compared
}

// get set
int TaskInfo::getPid(){
  return _pid;
}
void TaskInfo::setPid(int pid){
  _pid = pid;
}
bool TaskInfo::getIsAlive(){
  return _isAlive;
}
std::string TaskInfo::getName(){
  return _name;
}
double TaskInfo::getCpu(){
  return _cpu;
}
double TaskInfo::getMem(){
  return _mem;
}
double TaskInfo::getIoIn(){
  return _ioIn;
}
double TaskInfo::getIoOut(){
  return _ioOut;
}

// update info the first time
void TaskInfo::update0(){
  time(&_thisTime);
  // first assemble the root string
  std::ostringstream s;
  s << "/proc/" << _pid << "/";
  std::string fileRoot = s.str();
  // now we'll read the name.  We can check if the file exists here
  std::ifstream fName((fileRoot + "comm").c_str());
  if (fName.good()){
    _isAlive = true;
  } else {
    _isAlive = false;
    std::cout << "could not read name" << std::endl;
  }
  fName >> _name;
  fName.close();

  // now we'll read the mem
  std::ifstream fMem((fileRoot + "statm").c_str());
  if (fMem.good()){
    fMem >> _mem;
  } else {
    std::cout << "could not read memory" << std::endl;
  }
  fMem.close();

  // now we'll read the io
  // obviously it would be better to read twice to get the rate
  std::ifstream fIo((fileRoot + "io").c_str());
  if (fIo.good()){
    std::string tmp;
    fIo >> tmp;
    fIo >> _ioIn0;
    fIo >> tmp;
    fIo >> _ioOut0;
  } else {
    std::cout << "could not read io" << std::endl;
  }
  fIo.close();
  _ioIn = 0;
  _ioOut = 0;

  // cpu
  // the 14th and 15th field are user and kernel jiffys
  // again tihs is cummulative, really I need to do some averaging
  std::ifstream fCpu((fileRoot + "io").c_str());
  if (fCpu.good()){ 
    double tmp;
    std::string tmpS;
    fCpu >> tmp;
    //std::cout << "number 1 is " << tmp << std::endl;
    fCpu >> tmpS;
    //std::cout << "number 2 is " << tmpS << std::endl;
    fCpu >> tmpS;
    //std::cout << "number 3 is " << tmpS << std::endl;
    fCpu >> tmp;
    //std::cout << "number 4 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 5 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 6 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 7 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 8 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 9 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 10 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 11 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 12 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 13 is " << tmp << std::endl;
    fCpu >> _cpu0;
    //std::cout << "read raw cpu as " << _cpu0 << std::endl;
  } else {
    std::cout << "could not read cpu" << std::endl;
  }
  fCpu.close();
  _cpu = 0;
}

// now the real update
void TaskInfo::update(){
  _lastTime = _thisTime;
  time(&_thisTime);

  double deltaT = difftime(_thisTime, _lastTime);
  //std::cout << "deltaT is " << deltaT << std::endl;

  // first assemble the root string
  std::ostringstream s;
  s << "/proc/" << _pid << "/";
  std::string fileRoot = s.str();
  // now we'll read the name.  We can check if the file exists here
  std::ifstream fName((fileRoot + "comm").c_str());
  if (fName.good()){
    _isAlive = true;
  } else {
    _isAlive = false;
    std::cout << "could not read name" << std::endl;
  }
  fName >> _name;
  fName.close();

  // now we'll read the mem
  std::ifstream fMem((fileRoot + "statm").c_str());
  if (fMem.good()){
    fMem >> _mem;
  } else {
    std::cout << "could not read memory" << std::endl;
  }
  fMem.close();




  // now we'll read the io and get the rate
  std::ifstream fIo((fileRoot + "io").c_str());
  if (fIo.good()){
    std::string tmpS;
    fIo >> tmpS;
    fIo >> _ioIn;
    fIo >> tmpS;
    fIo >> _ioOut;

    double tmp = _ioIn;
    _ioIn = (_ioIn - _ioIn0)/deltaT;
    _ioIn0 = tmp;

    tmp = _ioOut;
    _ioOut = (_ioOut - _ioOut0)/deltaT;
    _ioOut0 = tmp;
  } else {
    std::cout << "could not read io" << std::endl;
  }
  fIo.close();

  // cpu
  // the 14th and 15th field are user and kernel jiffys
  // again tihs is cummulative, really I need to do some averaging
  std::ifstream fCpu((fileRoot + "stat").c_str());
  if (fCpu.good()){ 
    double tmp;
    std::string tmpS;
    fCpu >> tmp;
    //std::cout << "number 1 is " << tmp << std::endl;
    fCpu >> tmpS;
    //std::cout << "number 2 is " << tmpS << std::endl;
    fCpu >> tmpS;
    //std::cout << "number 3 is " << tmpS << std::endl;
    fCpu >> tmp;
    //std::cout << "number 4 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 5 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 6 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 7 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 8 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 9 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 10 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 11 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 12 is " << tmp << std::endl;
    fCpu >> tmp;
    //std::cout << "number 13 is " << tmp << std::endl;
    fCpu >> _cpu;
    //std::cout << "read raw cpu as " << _cpu << std::endl;

    tmp = _cpu;
    _cpu = (_cpu - _cpu0)/deltaT;
    _cpu0 = tmp;
  } else {
    std::cout << "could not read cpu" << std::endl;
  }
  fCpu.close();

}


// print info
void TaskInfo::print(){
  using namespace std;
  cout << "pid: "     << _pid     << endl;
  cout << "isAlive: " << _isAlive << endl;
  cout << "name: "    << _name    << endl;
  cout << "cpu: "     << _cpu     << endl;
  cout << "mem: "     << _mem     << endl;
  cout << "ioIn: "    << _ioIn    << endl;
  cout << "ioOut: "   << _ioOut   << endl;
}
