FLAG = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lpthread

edit: main.o color_local.o uart.o 
	g++ $(FLAG) -o edit main.o color_local.o uart.o 

main.o: main.cpp color_local.hpp uart.hpp 
	g++ -c main.cpp

uart.o: uart.cpp uart.hpp
	g++ -c uart.cpp

color.o:color_local.cpp 
	g++ -c color_local.cpp

clean:
	rm edit main.o color.o uart.o


