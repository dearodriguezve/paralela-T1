#include <opencv2/highgui.hpp>
#include <iostream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;
using namespace cv;

#define e  2.7182818284
#define sigma 3

float **gauss;
float filter = 15;          //tamano Kernel
Mat image;                  //Imagen Original
Mat blurImage;              //Iamgen Resultante
int lim = (filter - 1)/2;   //Radio Kernel
int THREADS;                //# Hilos

float matrixGauss()
{
  gauss = (float**) malloc(filter * sizeof(float *));
  
  for(int i = 0; i < filter; i++){
    gauss[i] = (float *) malloc(filter * sizeof(float));
  }
  
  // Se llena la matriz de convolucion gaussiana
  float sum = 0;
  float s = pow(sigma, 2);
  for(int i = -filter/2; i < filter/2; i++){
    for(int j = -filter/2; j < filter/2; j++){
    gauss[i+int(filter/2)][j+int(filter/2)] = (1/(2*M_PI*s))*(pow(e,(-(i*i+j*j)/(2*s))));
    sum += gauss[i+int(filter/2)][j+int(filter/2)];
    }
  }
  
  // Normalizacion
  for(int i = 0; i < filter; i++){
    for(int j = 0; j < filter; j++){
      gauss[i][j] /=sum;
    }
  }    
}

void *convolucion(void *arg)
{
  int threadId = *(int *)arg;
  int columns = image.cols;
  int rows = image.rows;
  for(int y = threadId*(rows/THREADS); y < (((threadId+1)*(rows/THREADS))); y++){        
    for (int x = 0; x < columns; x++){
      int val0= 0;
      int val1= 0;
      int val2= 0;
      for (int i = 0; i < filter; i++){
        for (int j = 0; j < filter; j++){
          // Manejo de bordes
          if( (y-lim+i < 0 ||y-lim+i > rows) || (x-lim+j < 0 ||x-lim+j > columns) ){
          }else{
            // Se leen valores RGB y se operan con el kernel gaussiano
            val0+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[0]*gauss[i][j]);
            val1+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[1]*gauss[i][j]);
            val2+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[2]*gauss[i][j]);
          }
        }
      }
      // Sobrescritura de la imagen de salida
      blurImage.at<Vec3b>(y, x).val[0] = val0;
      blurImage.at<Vec3b>(y, x).val[1] = val1;
      blurImage.at<Vec3b>(y, x).val[2] = val2;
    }  
  }
  return 0;
}

int main(int argc, char *argv[]) 
{
  timespec ts1,ts2;
  clock_gettime(CLOCK_REALTIME, &ts1);
  char *entrada = argv[1];
  char *salida = argv[2];
  filter = atoi(argv[3]);
  THREADS = atoi(argv[4]);
  
  matrixGauss();

  image = imread(entrada , CV_LOAD_IMAGE_COLOR);
  blurImage = image.clone();

  if(! image.data ) {
    cout <<  "Could not open or find the image" << endl ;
    return -1;
  }
  
  int threadId[THREADS], i, *retval;
  pthread_t thread[THREADS];

  // Creacion de hilos
  for(i = 0; i < THREADS; i++){
    threadId[i] = i;
    pthread_create(&thread[i], NULL, (void *)convolucion, &threadId[i]);
  }
  // Sincronizacion de hilos
  for(i = 0; i < THREADS; i++){
    pthread_join(thread[i], (void **)&retval);
  }
  
  imwrite(salida, blurImage);
  free(gauss);
  clock_gettime(CLOCK_REALTIME, &ts2);
  printf("%ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec), abs(ts2.tv_nsec - ts1.tv_nsec));
  return 0;
}