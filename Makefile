all: taskmon



taskmon: taskmon.o taskInfo.o
	g++ -o taskmon taskmon.o taskInfo.o


taskmon.o: taskmon.cpp taskInfo.hpp
	g++ -c -o taskmon.o taskmon.cpp

taskInfo.o: taskInfo.cpp taskInfo.hpp
	g++ -c -o taskInfo.o taskInfo.cpp
