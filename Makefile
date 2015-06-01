all: taskmon



taskmon: taskmon.o
	g++ -o taskmon taskmon.o

taskmon.o: taskmon.cpp
	g++ -c -o taskmon.o taskmon.cpp
