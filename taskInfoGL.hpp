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
  double _z;

  // and velocity
  double _xdot;
  double _ydot;
  double _zdot;

  // and a size
  double _s;
  // and a mass
  double _m;

  // colors
  double _r;
  double _g;
  double _b;

  void setDefaults();
 public:
  // constructor
  TaskInfoGL();
  TaskInfoGL(int pid);
  TaskInfoGL(int pid, double x, double y, double z);

  // get and set
  void setX(double x);
  double getX() const;
  void setY(double y);
  double getY() const;
  void setZ(double z);
  double getZ() const;

  void setXdot(double x);
  double getXdot() const;
  void setYdot(double y);
  double getYdot() const;
  void setZdot(double z);
  double getZdot() const;




  void setS(double s);
  double getS() const;

  void setM(double m);
  double getM() const;

  void setR(double r);
  double getR() const;
  void setG(double g);
  double getG() const;
  void setB(double b);
  double getB() const;

  // update function
  void updateState(double Fx = 0.0, double Fy = 0.0, double Fz = 0.0);

};

#endif
