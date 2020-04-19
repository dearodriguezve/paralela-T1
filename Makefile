all:
	g++ blur-effect.cpp -fopenmp -fpermissive -lpthread  -o output `pkg-config --cflags --libs opencv`