# paralela-T1
dafguerreroal-jrgalvisg-dearodriguezve
## Se requiere la libreria open cv
## se compila con g++
## el comando de compilacion es g++ blur-effect.cpp -fopenmp -fpermissive -lpthread  -o output `pkg-config --cflags --libs opencv`
## el comando de ejecuacion es ./output imagenDeEntreda imabgenDeSalida TamanioKernel #Hilos
## con el archivo Makefile se compila el archivo cpp y con script.sh se ejecutan todos los casos