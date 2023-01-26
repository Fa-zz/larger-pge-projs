CXX = g++
CXXFLAGS = -Wall

main: main.cpp olcPixelGameEngine.h stringButton.cpp
	$(CXX) $(CXXFLAGS) -o main \
		-I/usr/X11/include -L/usr/X11/lib -lX11 -lGL -lpng -lpthread -std=c++17 main.cpp

stringButton: stringButton.cpp stringButton.hpp
	$(CXX) $(CXXFLAGS) -o stringButton -std=c++17 stringButton.cpp


# stringButton: stringButton.cpp stringButton.hpp
# 	$(CXX) $(CXXFLAGS) -o stringButton \
# 		-I/usr/X11/include -L/usr/X11/lib -lX11 -lGL -lpng -lpthread -std=c++17 stringButton.cpp


clean:
	rm main

run:
	./main

# g++ -Wall -o main main.cpp \
#   -I/usr/X11/include -L/usr/X11/lib -lX11 -lGL -lpng -lpthread -std=c++17