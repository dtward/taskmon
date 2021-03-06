#include "taskInfoGL.hpp"
// constructors
TaskInfoGL::TaskInfoGL()
  : TaskInfo()
{
  setDefaults();
}

TaskInfoGL::TaskInfoGL(int pid)
  : TaskInfo(pid)
{
  setDefaults();
}

TaskInfoGL::TaskInfoGL(int pid, double x, double y, double z)
  : TaskInfo(pid)
{
  setDefaults();
  _x = x;
  _y = y;
  _z = z;
}



// defaults
void TaskInfoGL::setDefaults(){
  _x = 0.0;
  _y = 0.0;
  _z = 0.0;

  _xdot = 0.0;
  _ydot = 0.0;
  _zdot = 0.0;

  _s = 0.1;
  _m = 1.0;

  _r = 0.75*0; // let's start at zero and increase over time
  _g = 0.75*0;
  _b = 0.75*0;

  _thisTimeState = 10;
  _lastTimeState = 0.0;
}


// update
void TaskInfoGL::updateState( double Fx, double Fy, double Fz){
  // set the size and mass based on the task properties
  // I really should do lowpass filtering here!
  // for example
  double p = 0.05;
  double cpuScale = 1.0;
  _s = _s*(1.0 - p) + getCpu()*cpuScale*p;
  std::cout << "cpu is " << getCpu() << ", scale is " << _s << std::endl;

  double memScale = 1e-4;
  _m = _m*(1.0 - p) + getMem()*memScale*p;
  //_m = 1.0;
  //_m = 0.1;



  // set the r,g,b
  // not sure what to set them based on

  // update the dynamics
  _lastTimeState = _thisTimeState;
  _thisTimeState = clock();
  double deltat = (_thisTimeState - _lastTimeState)*1.0/CLOCKS_PER_SEC;
  _x += _xdot * deltat;
  _y += _ydot * deltat;
  _z += _zdot * deltat;

  _xdot += Fx/_m * deltat;
  _ydot += Fy/_m * deltat;
  _zdot += Fz/_m * deltat;

  std::cout << "_xdot increased by " << Fx/_m * deltat << std::endl;


  _r += (1.0 - _r)*deltat*0;
  _g += (1.0 - _g)*deltat; // when _g = 1, the derivative is zero
  _b += (1.0 - _b)*deltat;

  _g = (_g>1) ? 1 : ( (_g < 0) ? 0 : _g );
  _r = (_r>1) ? 1 : ( (_r < 0) ? 0 : _r );
  _b = (_b>1) ? 1 : ( (_b < 0) ? 0 : _b );


}




// get set
void TaskInfoGL::setX(double x){
  _x = x;
}
double TaskInfoGL::getX() const{
  return _x;
}
void TaskInfoGL::setY(double y){
  _y = y;
}
double TaskInfoGL::getY() const{
  return _y;
}
void TaskInfoGL::setZ(double z){
  _z = z;
}
double TaskInfoGL::getZ() const{
  return _z;
}

void TaskInfoGL::setXdot(double xdot){
  _xdot = xdot;
}
double TaskInfoGL::getXdot() const{
  return _xdot;
}
void TaskInfoGL::setYdot(double ydot){
  _ydot = ydot;
}
double TaskInfoGL::getYdot() const{
  return _ydot;
}
void TaskInfoGL::setZdot(double zdot){
  _zdot = zdot;
}
double TaskInfoGL::getZdot() const{
  return _zdot;
}


void TaskInfoGL::setS(double s){
  _s = s;
}
double TaskInfoGL::getS() const{
  return _s;
}

void TaskInfoGL::setM(double m){
  _m = m;
}
double TaskInfoGL::getM() const{
  return _m;
}





void TaskInfoGL::setR(double r){
  _r = r;
}
double TaskInfoGL::getR() const{
  return _r;
}
void TaskInfoGL::setG(double g){
  _g = g;
}
double TaskInfoGL::getG() const{
  return _g;
}
void TaskInfoGL::setB(double b){
  _b = b;
}
double TaskInfoGL::getB() const{
  return _b;
}
