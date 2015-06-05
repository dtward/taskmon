all: taskmon



taskmon: taskmon.o taskInfo.o taskInfoGL.o
	g++ -o taskmon taskmon.o taskInfo.o taskInfoGL.o -lGL -lglut -pthread


taskmon.o: taskmon.cpp taskInfo.hpp taskInfoGL.hpp
	g++ -c -o taskmon.o taskmon.cpp

taskInfo.o: taskInfo.cpp taskInfo.hpp
	g++ -c -o taskInfo.o taskInfo.cpp

taskInfoGL.o: taskInfoGL.cpp taskInfo.hpp 
	g++ -c -o taskInfoGL.o taskInfoGL.cpp

clean:
	rm -f taskmon
	rm -f *.o
