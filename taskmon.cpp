#include <iostream>


#include "GL/glut.h"

#include "unistd.h" // for sleep
#include <vector>
#include <string>
#include "dirent.h" // basic directory reader
#include <sstream>
#include <algorithm> // for sort, find
#include <cmath> // for finding force

#include "taskInfo.hpp"
#include "taskInfoGL.hpp"



// glut will use global variables
struct GlutWindow {
  int width;
  int height;
  int x;
  int y;
  int mode;
  std::string name;
  int id;
  int msec;
  int timerValue;
  // default constructor will set to the parameters I want
  GlutWindow(){
    width = 500;
    height = 500;
    x = 100;
    y = 100;
    mode = GLUT_DOUBLE | GLUT_RGB;
    name = "test";
    id = 0;
    msec = 1.0/30.0*1000*10;
    timerValue = 0;
  }
} glutWindow;

void display();
void timer(int value);



// my tasks will have to be global
std::vector<TaskInfoGL> tasks; // maybe better to use a list

// function to update a vector of tasks
void updateTasks(std::vector<TaskInfoGL> & tasks){
  int count = 0;
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
    for (std::vector<TaskInfoGL>::iterator pt = tasks.begin(); pt != tasks.end(); ++pt){
      if (pid == pt->getPid()){
	alreadyHaveThisTask = true;
	break;
      }
    } // end of loops to check if we already have this task in our vector
    if (!alreadyHaveThisTask){
      double x,y,z;
      // let's hope we don't put them right in the same place
      count++;
      x = count/1000.0*(count > 5);
      y = count/1000.0;
      z = 0;
      tasks.push_back( TaskInfoGL(pid,x,y,z) );
    }
  } // end loop over tasks in proc directory



    // clear
  for (int i = 0; i < 10; i++){
    std::cout << std::endl;
  }

    
  // clean up my vector
  std::cout << "size of tasks vector before cleanup: " << tasks.size() << std::endl;
  std::vector<TaskInfoGL>::iterator it = tasks.begin();
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
  for (std::vector<TaskInfoGL>::reverse_iterator pt = tasks.rbegin(); pt < tasks.rbegin() + 10; ++pt){ // begin loop over tasks in my vector
    std::cout << pt->getName() << ": cpu " << pt->getCpu() << ", mem " << pt->getMem() << std::endl; 
  } // end loop over tasks in my vector
}


int main(int argc, char** argv){

  std::cout << "Hello world" << std::endl;

  // set what to sort by
  // default by cpu
  TaskInfo::setSortBy(TaskInfo::CPU);
  // check input arguments
  std::vector<std::string>inputArgs(argv,argv+argc);
  for (std::vector<std::string>::iterator it = inputArgs.begin(); it != inputArgs.end(); ++it){
    std::cout << " " << *it;
  }
  std::cout << std::endl;


  // add more checks here if I want to sort by other things
  std::vector<std::string>::iterator memInd = std::find(inputArgs.begin(),inputArgs.end(),"mem");
  std::vector<std::string>::iterator cpuInd = std::find(inputArgs.begin(),inputArgs.end(),"cpu");
  if (memInd != inputArgs.end()){
    TaskInfo::setSortBy(TaskInfo::MEM);
    std::cout << "sorting by memory use" << std::endl;
  } else if (cpuInd != inputArgs.end()) {
    TaskInfo::setSortBy(TaskInfo::CPU);
    std::cout << "sorting by cpu use" << std::endl;
  }



  // glut setup
  glutInit(&argc,argv);
  glutInitWindowSize(glutWindow.width,glutWindow.height);
  glutInitWindowPosition(glutWindow.x,glutWindow.y);
  glutInitDisplayMode(glutWindow.mode);
  glutWindow.id = glutCreateWindow(glutWindow.name.c_str());
  // callbacks
  glutDisplayFunc(&display);
  glutTimerFunc(glutWindow.msec,&timer,glutWindow.timerValue); // we can pass an integer value
  // initialize state
  glClearColor(0.0,0.0,0.0,0.0);


  // start the program
  updateTasks(tasks);
  glutMainLoop();

  // begin the work, this will likely be replaced when I get a window
  int nLoop = 10; // may switch this for a while loop


  for (int i = 0; i < nLoop; i++){ // start loop over repeated printing
    updateTasks(tasks);
    // sleep before doing stuff
    sleep(2);
  } // end of loop over repeated printing

  // note that we measure time in seconds!
  // so we're doing an average over one second or more

  return 0;
}




// display function
void display(){
  // clear window
  glClear(GL_COLOR_BUFFER_BIT);

  // set up view
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,-1,1); // left right bottom top near far
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  // draw
  std::cout << "about to draw tasks" << std::endl;
  for (std::vector<TaskInfoGL>::iterator it = tasks.begin(); it != tasks.end(); ++it){
    std::cout << "drawing " << it->getName()  << " at (";

    glBegin(GL_TRIANGLES);
    double x,y,z;
    x = it->getX();
    y = it->getY();
    z = it->getZ();
    std::cout << x << ", " << y << ", " << z << ")" << std::endl;
    glColor3f(it->getR(),it->getG(),it->getB());
    glVertex3f(x,y,z);
    glVertex3f(x+0.1,y,z);
    glVertex3f(x,y+0.1,z);
    glEnd();
  }


  // swap buffers
  glutSwapBuffers();
}



// timer function
int count = 0;
void timer(int value){
  count ++ ;
  std::cout << "In timer function, count is " << count << std::endl;

  // update the task values every few seconds
  if (!count%100)
    updateTasks(tasks); 

  // loop over tasks
  for (std::vector<TaskInfoGL>::iterator it0 = tasks.begin(); it0 != tasks.end(); ++it0){
    // calculate force
    double fx,fy,fz;
    fx = fy = fz = 0.0;
    double d2,d;
    double x0,y0,z0,x1,y1,z1,dx,dy,dz,nx,ny,nz;
    x0 = it0->getX();
    y0 = it0->getY();
    z0 = it0->getZ();
    for (std::vector<TaskInfoGL>::iterator it1 = tasks.begin(); it1 != tasks.end(); ++it1){
      if (it1 == it0) continue;
      x1 = it1->getX();
      y1 = it1->getY();
      z1 = it1->getZ();
      dx = x1 - x0;
      dy = y1 - y0;
      dz = z1 - z0;
      d2 = dx*dx + dy*dy + dz*dz;
      d = sqrt(d2);
      nx = dx/d;
      ny = dy/d;
      nz = dz/d;

      // now we have a direction for the force
      double restLength = 0.2;
      double f = (d - restLength)*0.1;
      fx += f*nx;
      fy += f*ny;
      fz += f*nz;
    }
    // update state
    std::cout << "updating state with force (" << fx << ", " << fy << ", " << fz << ")" << std::endl;
    it0->updateState(fx,fy,fz);
  }
  glutTimerFunc(glutWindow.msec,&timer,value);
  glutPostRedisplay();
}
