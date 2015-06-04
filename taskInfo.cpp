#include "taskInfo.hpp"

// I need to wait some time and measure twice
// think about how to do this
// probably get the initial values in the constructor
// then update is a different function


// default sort by
// this is the weird notation to set static members
TaskInfo::property TaskInfo::_sortBy = TaskInfo::CPU;
// default verbose
bool TaskInfo::_verbose = false;


// constructor
TaskInfo::TaskInfo(){
  _isAlive = false; // do nothing
}
TaskInfo::TaskInfo(int pid){
  _pid = pid;
  update0(); // first time only, get things that need to be compared
}

// get set
int TaskInfo::getPid() const {
  return _pid;
}
void TaskInfo::setPid(int pid) {
  _pid = pid;
}
bool TaskInfo::getIsAlive() const {
  return _isAlive;
}
std::string TaskInfo::getName() const {
  return _name;
}
double TaskInfo::getCpu() const {
  return _cpu;
}
double TaskInfo::getMem() const {
  return _mem;
}
double TaskInfo::getIoIn() const {
  return _ioIn;
}
double TaskInfo::getIoOut() const {
  return _ioOut;
}
TaskInfo::property TaskInfo::getSortBy() {
  return _sortBy;
}
void TaskInfo::setSortBy(TaskInfo::property p){
  _sortBy = p;
}


// update info the first time
void TaskInfo::update0(){
  // set to true until we see otherwise
  _isAlive = true;
  // get the time
  time(&_thisTime);
  // first assemble the root string
  std::ostringstream s;
  s << "/proc/" << _pid << "/";
  std::string fileRoot = s.str();
  // now we'll read the name.  We can check if the file exists here
  std::ifstream fName((fileRoot + "comm").c_str());
  if (fName.good()){
  } else {
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read name" << std::endl;
    }
  }
  fName >> _name;
  fName.close();

  // now we'll read the mem
  std::ifstream fMem((fileRoot + "statm").c_str());
  if (fMem.good()){
    fMem >> _mem;
  } else {
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read memory" << std::endl;
    }
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
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read io" << std::endl;
    }
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
    if (_verbose){
      std::cerr << "could not read cpu" << std::endl;
    }
    _isAlive = false;
  }
  fCpu.close();
  _cpu = 0;
}

// now the real update
void TaskInfo::update(){
  // set to true until we find otherwise
  _isAlive = true;
  // get time
  _lastTime = _thisTime;
  time(&_thisTime);
  double deltaT = difftime(_thisTime, _lastTime);
  if (deltaT == 0) deltaT = 10000.0; // avoid an error, make the numbers really small the first time
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
    if (_verbose){
      std::cerr << "could not read name" << std::endl;
    }
  }
  fName >> _name;
  fName.close();

  // now we'll read the mem
  std::ifstream fMem((fileRoot + "statm").c_str());
  if (fMem.good()){
    fMem >> _mem;
  } else {
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read memory" << std::endl;
    }
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
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read io" << std::endl;
    }
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
    _isAlive = false;
    if (_verbose){
      std::cerr << "could not read cpu" << std::endl;
    }
  }
  fCpu.close();

}


// print info
void TaskInfo::print() const {
  using namespace std;
  cout << "pid: "     << _pid     << endl;
  cout << "isAlive: " << _isAlive << endl;
  cout << "name: "    << _name    << endl;
  cout << "cpu: "     << _cpu     << endl;
  cout << "mem: "     << _mem     << endl;
  cout << "ioIn: "    << _ioIn    << endl;
  cout << "ioOut: "   << _ioOut   << endl;
}



// comparisson for sorting
bool TaskInfo::operator<(TaskInfo const & t) const {
  switch (_sortBy){
  case CPU:
    //std::cout << "comparing by cpu" << std::endl;
    return _cpu < t._cpu;
    break;
  case MEM:
    //std::cout << "comparing by mem" << std::endl;
    return _mem < t._mem;
    break;
  }
}

bool TaskInfo::operator>( TaskInfo const & t) const {
  return !operator<(t);
}
