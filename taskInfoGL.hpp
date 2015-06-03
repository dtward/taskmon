#ifndef TASK_INFO_GL_H
#define TASK_INFO_GL_H

#include "taskInfo.hpp"

class TaskInfoGL : 
public TaskInfo
{
 private:
  // members include an x coordinate and a y coordinate
  double _x;
  double _y;

  // and velocity
  double _xdot;
  double _ydot;

  // and a size
  double _s;
  // and a mass
  double _m;


  void setDefaults();
 public:
  // constructor
  TaskInfoGL();
  TaskInfoGL(int pid);
  TaskInfoGL(int pid, double x, double y);

  // get and set
  void setX(double x);
  double getX();
  void setY( double y);
  double getY();


  void setS(double s);
  double getS();

  void setM(double m);
  double getM();


  // update function
  void update();

};

#endif
