#include <iostream>

#include "taskInfo.hpp"

#include "unistd.h" // for sleep
#include <vector>
#include "dirent.h" // basic directory reader
#include <sstream>

int 
main(int argc, char** argv){

  std::cout << "Hello world" << std::endl;


  // can I loop over a directory?
  std::vector<TaskInfo> tasks;
  DIR * pdir = opendir("/proc/");
  dirent * p;
  while ( (p = readdir(pdir)) != NULL){
    std::istringstream s(p->d_name);
    int pid;
    s >> pid;
    std::cout << pid << std::endl;

    tasks.push_back( TaskInfo(pid) );
  }

  //TaskInfo t(32531);
  //t.print();



  for (int i = 0; i < 10; i++){
    sleep(2); 
    for (std::vector<TaskInfo>::iterator pt = tasks.begin(); pt != tasks.end(); ++pt){
      
      pt->update();
      if (pt->getIsAlive()){
	//pt->print();
	std::cout << pt->getName() << ": cpu " << pt->getCpu() << ", mem " << pt->getMem() << std::endl; 
      }
    }
  }
  // note that we measure time in seconds!
  // so we're doing an average over one second or more

  return 0;
}
