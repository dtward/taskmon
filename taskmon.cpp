#include <iostream>

#include "taskInfo.hpp"

#include "unistd.h" // for sleep
#include <vector>
#include "dirent.h" // basic directory reader
#include <sstream>
#include <algorithm> // for sort

int 
main(int argc, char** argv){

  std::cout << "Hello world" << std::endl;


  TaskInfo::setSortBy(TaskInfo::MEM);

  int nLoop = 10; // may switch this for a while loop
  std::vector<TaskInfo> tasks; // maybe better to use a list

  for (int i = 0; i < nLoop; i++){ // start loop over repeated printing
    // loop over a directory
    DIR * pdir = opendir("/proc/");
    dirent * p;
    while ( (p = readdir(pdir)) != NULL){ // begin loop over tasks in proc directory
      std::istringstream s(p->d_name);
      int pid;
      s >> pid;
      //std::cout << pid << std::endl;

      // check if we already have this task in our list
      bool alreadyHaveThisTask = false;
      for (std::vector<TaskInfo>::iterator pt = tasks.begin(); pt != tasks.end(); ++pt){
	if (pid == pt->getPid()){
	  alreadyHaveThisTask = true;
	  break;
	}
      } // end of loops to check if we already have this task in our vector
      if (!alreadyHaveThisTask){
	tasks.push_back( TaskInfo(pid) );
      }
    } // end loop over tasks in proc directory


    // sleep before doing stuff
    sleep(2);

    // clear
    for (int i = 0; i < 10; i++){
      std::cout << std::endl;
    }

    
    // clean up my vector
    std::cout << "size of tasks vector before cleanup: " << tasks.size() << std::endl;
    std::vector<TaskInfo>::iterator it = tasks.begin();
    while (it != tasks.end()){
      it->update();
      if (!it->getIsAlive()){
	it = tasks.erase(it);
      } else {
	++it;
      }
    }
    std::cout << "size of tasks vector after cleanup: " << tasks.size() << std::endl;



    // sort it!
    std::sort(tasks.begin(),tasks.end());



    
    //for (std::vector<TaskInfo>::reverse_iterator pt = tasks.rbegin(); pt != tasks.rend(); ++pt){ // begin loop over tasks in my vector
    for (std::vector<TaskInfo>::reverse_iterator pt = tasks.rbegin(); pt < tasks.rbegin() + 10; ++pt){ // begin loop over tasks in my vector
      std::cout << pt->getName() << ": cpu " << pt->getCpu() << ", mem " << pt->getMem() << std::endl; 
    } // end loop over tasks in my vector



  } // end of loop over repeated printing

  // note that we measure time in seconds!
  // so we're doing an average over one second or more

  return 0;
}
