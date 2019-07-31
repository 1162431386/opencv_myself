FLAG =-L ./lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lpthread -lopencv_imgcodecs -llept -ltesseract -lm -Wall

edit: main.o 
	g++ $(FLAG) -g -o edit main.o 
main.o: main.cpp 
	g++ -c main.cpp 
#image.o: image.cpp color.hpp
#	g++ -c image.cpp
#uart.o: uart.cpp uart.hpp
#	g++ -c uart.cpp
#pid.o:pid.cpp pid.hpp
#	g++ -c pid.cpp 

clean:
	rm -f edit.o main.o 
