all:
	g++ main1.cpp -fpermissive -lpthread  -o output1 `pkg-config --cflags --libs opencv