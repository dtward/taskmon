#include "taskInfoGL.hpp"
// constructors
TaskInfoGL::TaskInfoGL()
  : TaskInfo()
{
  _x = 0.0;
  _y = 0.0;
}

TaskInfoGL::TaskInfoGL(int pid)
  : TaskInfo(pid)
{
  _x = 0.0;
  _y = 0.0;
}

TaskInfoGL::TaskInfoGL(int pid, double x, double y)
  : TaskInfo(pid)
{
  _x = x;
  _y = y;
}



// defaults
void TaskInfoGL::setDefaults(){
  _x = 0.0;
  _y = 0.0;

  _xdot = 0.0;
  _ydot = 0.0;

  _s = 0.1;
  _m = 1.0;

}


// update
void TaskInfoGL::update(){
  TaskInfo::update();

  double deltat = _thisTime - _lastTime;
  _x += _xdot * deltat;
  _y += _ydot * deltat;


}
