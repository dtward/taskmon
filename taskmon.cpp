
// standard library includes
#include <iostream>
#include "unistd.h" // for sleep
#include <vector>
#include <string>
#include "dirent.h" // basic directory reader
#include <sstream>
#include <algorithm> // for sort, find
#include <cmath> // for finding force, need sqrt

// to get things working at work, seems I need to include pthread
// as in here: http://stackoverflow.com/questions/20007961/error-running-a-compiled-c-file-uses-opengl-error-inconsistency-detected
#include "pthread.h"
void junk(){
  int i;
  i = pthread_getconcurrency();
}


// includes for this project
#include "GL/glut.h"
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
  int nTasks;
  // default constructor will set to the parameters I want
  GlutWindow(){
    width = 500;
    height = 500;
    x = 100;
    y = 100;
    mode = GLUT_DOUBLE | GLUT_RGB;
    name = "test";
    id = 0;
    msec = 1.0/30.0*1000*2.0;
    timerValue = 0;
    nTasks = 25;
    nTasks = 200;
    //nTasks = 100;
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
      x = cos(count*2.0*3.14159/glutWindow.nTasks)*0.5;
      y = sin(count*2.0*3.14159/glutWindow.nTasks)*0.5;
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

  int count = 0;
  for (std::vector<TaskInfoGL>::reverse_iterator it = tasks.rbegin(); it != tasks.rend(); ++it){
    count++;
    if (it - glutWindow.nTasks == tasks.rbegin()) break;
    std::cout << "drawing task " << count << ", " << it->getName()  << " at (";
    double x,y,z;
    x = it->getX();
    y = it->getY();
    z = it->getZ();
    std::cout << x << ", " << y << ", " << z << ")" << std::endl;


    glPushMatrix();

    glTranslatef(x,y,z);

    double s = it->getS(); // note that we are on the order of 10^6 the first time
    // after first time we are on the order of 10 to 50?
    s *= 0.01;
    if (s > 0.3) s = 0.3;
    if (s < 0.05) s = 0.05;
    //s = 0.1; 
    std::cout << "scale is " << s << std::endl;

    glPushMatrix();
    glScalef(s,s,s);

    glBegin(GL_TRIANGLES);
    glColor3f(it->getR(),it->getG(),it->getB());
    //glColor3f(0.25,0.5,0.25);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(0,1,0);
    glEnd();


    glColor3f(1,1,1);
    glRasterPos3f(0.125,0.25,0); 
    std::string myName = it->getName();
    void * font = GLUT_BITMAP_HELVETICA_18;
    for (std::string::iterator i = myName.begin(); i != myName.end(); ++i){
      glutBitmapCharacter(font,*i);
    }
    //glutBitmapCharacter(font,'\n'); // this doesn't seem to do anything
    glRasterPos3f(0.125,0.0,0); 

    std::ostringstream sCPU;
    sCPU << "CPU: " << it->getCpu();
    std::string myCPU = sCPU.str();
    for (std::string::iterator i = myCPU.begin(); i != myCPU.end(); ++i){
      glutBitmapCharacter(font,*i);
    }

    glPopMatrix();
    glPopMatrix();
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
  if (!(count%20)){
    std::cout << "count is " << count << ", updating tasks" << std::endl;
    updateTasks(tasks); 
  }

  // loop over tasks
  for (std::vector<TaskInfoGL>::reverse_iterator it0 = tasks.rbegin(); it0 != tasks.rend(); ++it0){
    if (it0 - glutWindow.nTasks == tasks.rbegin()) break;
    // calculate force
    double fx,fy,fz;
    fx = fy = fz = 0.0;
    double d2,d;
    double x0,y0,z0,x1,y1,z1,dx,dy,dz,nx,ny,nz;
    x0 = it0->getX();
    y0 = it0->getY();
    z0 = it0->getZ();
    for (std::vector<TaskInfoGL>::reverse_iterator it1 = tasks.rbegin(); it1 != tasks.rend(); ++it1){
      if (it1 == it0) continue;
      if (it1 - glutWindow.nTasks == tasks.rbegin()) break;
      x1 = it1->getX();
      y1 = it1->getY();
      z1 = it1->getZ();
      dx = x1 - x0;
      dy = y1 - y0;
      dz = z1 - z0;
      d2 = dx*dx + dy*dy + dz*dz;
      d = sqrt(d2);
      double ep = 0.00001;
      nx = dx/(d+ep);
      ny = dy/(d+ep);
      nz = dz/(d+ep);

      // now we have a direction for the spring force
      // I really want the spring length to depend on  the scale
      double restLength = 0.75;
      restLength = (it0->getS() + it1->getS()) * 0.00001;
      restLength = (restLength > 0.3) ? 0.3 :
	( (restLength < 0.05) ? 0.05 : restLength);
      double f = (d - restLength)*1.0/glutWindow.nTasks*10;
      fx += f*nx;
      fy += f*ny;
      fz += f*nz;
    }
    // add a force pulling you back to the center
    fx -= it0->getX()*0.1;
    fy -= it0->getY()*0.1;
    fz -= it0->getZ()*0.1;
    // need a drag force
    //double v = sqrt( pow(it0->getXdot(), 2) + pow(it0->getYdot(),2) + pow(it0->getZdot(),2) );
    fx -= it0->getXdot()*0.1*10;
    fy -= it0->getYdot()*0.1*10;
    fz -= it0->getZdot()*0.1*10;

    // update state
    //fx = fy = fz = 0;
    std::cout << "updating state with force (" << fx << ", " << fy << ", " << fz << ")" << std::endl;
    it0->updateState(fx,fy,fz);
    double out = 0.9;
    if (it0->getX() > out) {it0->setX(out);it0->setXdot(0);}
    if (it0->getY() > out) {it0->setY(out);it0->setYdot(0);}
    if (it0->getZ() > out) {it0->setZ(out);it0->setZdot(0);}
    if (it0->getX() < -out) {it0->setX(-out);it0->setXdot(0);}
    if (it0->getY() < -out) {it0->setY(-out);it0->setYdot(0);}
    if (it0->getZ() < -out) {it0->setZ(-out);it0->setZdot(0);}
  }
  glutTimerFunc(glutWindow.msec,&timer,value);
  glutPostRedisplay();
}
