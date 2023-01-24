CXX = g++
CXXFLAGS = -Wall

main: main.cpp olcPixelGameEngine.h
	$(CXX) $(CXXFLAGS) -o main \
		-I/usr/X11/include -L/usr/X11/lib -lX11 -lGL -lpng -lpthread -std=c++17 main.cpp

clean:
	rm main

run:
	./main

# g++ -Wall -o main main.cpp \
#   -I/usr/X11/include -L/usr/X11/lib -lX11 -lGL -lpng -lpthread -std=c++17