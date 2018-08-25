CXX = g++ -O2 -Wall

all:
	$(CXX) `pkg-config --cflags opencv` -O2 main.cpp Color.cpp Noise.cpp Filter.cpp `pkg-config --libs opencv` -o main.out

clean:
	rm -f main.out *.o