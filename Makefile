all:
	g++ blur-effect.cpp -fpermissive -lpthread  -o output `pkg-config --cflags --libs opencv`