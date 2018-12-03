FLAG = -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio

edit: main.o uart.o color1.o color2.o 
	g++ $(FLAG) -o edit main.o uart.o color1.o color2.o 
main.o: main.cpp color.hpp
	g++ -c main.cpp
uart.o: uart.cpp uart.hpp -lm
	g++ -c uart.cpp
color1.o: color1.cpp color.hpp
	g++ -c color1.cpp
color2.o: color2.cpp color.hpp
	g++ -c color2.cpp
clean:
	rm edit main.o uart.o color1.o color2.o

run:
	pkill edit || ./edit
