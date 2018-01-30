all:main.o Read_File.o Simu.o Segment.o
	g++ -o Dectector main.o Read_File.o Simu.o Segment.o
main.o:main.cpp
	g++ -c main.cpp
Segment.o:Segment.cpp
	g++ -c Segment.cpp
Simu.o:Simu.cpp
	g++ -c Simu.cpp
Read_File.o:Read_File.cpp
	g++ -c Read_File.cpp

clean:
	rm Dectector main.o Read_File.o Simu.o Segment.o
