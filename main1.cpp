#include <opencv2/highgui.hpp>
#include <iostream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
 #include <pthread.h>

using namespace std;
using namespace cv;

float **gauss;
float filter =15;
Mat image;
Mat blurImage;
int lim = (filter - 1)/2;

#define e  2.7182818284
#define sigma 3
#define THREADS 12

float matrixGauss()
{
    gauss = (float**) malloc(filter * sizeof(float *));
    
    for(int i = 0; i < filter; i++){
        gauss[i] = (float *) malloc(filter * sizeof(float));
    }
    
    // Se llena la matriz de convolucion gaussiana
    float sum = 0;
    float s = pow(sigma, 2);
    //printf("%0.2f",s);
    for(int i = -filter/2; i < filter/2; i++){
        for(int j = -filter/2; j < filter/2; j++){
            gauss[i+int(filter/2)][j+int(filter/2)] = (1/(2*M_PI*s))*(pow(e,(-(i*i+j*j)/(2*s))));
            sum += gauss[i+int(filter/2)][j+int(filter/2)];
        }
    }
    
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
      Vec3b intensity = image.at<Vec3b>(y, x);
      int val0= 0;
      int val1= 0;
      int val2= 0;
      for (int i = 0; i < filter; i++){
        for (int j = 0; j < filter; j++){
          if( (y-lim+i < 0 ||y-lim+i > rows) || (x-lim+j < 0 ||x-lim+j > columns) ){
            
          }else{
            val0+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[0]*gauss[i][j]);
            val1+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[1]*gauss[i][j]);
            val2+= (image.at<Vec3b>(y-lim+i, x-lim+j).val[2]*gauss[i][j]);
          }
        }
      }
      blurImage.at<Vec3b>(y, x).val[0]= val0;
      blurImage.at<Vec3b>(y, x).val[1]= val1;
      blurImage.at<Vec3b>(y, x).val[2]= val2;
    }  
  }
  return 0;
}

int main() 
{
 
  matrixGauss();
  for(int i = 0; i < filter; i++){
      for(int j = 0; j < filter; j++){
          printf("   %0.8f",gauss[i][j]);
      }
      printf("\n");
  }
  image = imread("pasisaje.jpeg" , CV_LOAD_IMAGE_COLOR);
  blurImage = image.clone();

  if(! image.data ) {
    cout <<  "Could not open or find the image" << endl ;
    return -1;
  }
  
  int threadId[THREADS], i, *retval;
    pthread_t thread[THREADS];

    for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        pthread_create(&thread[i], NULL, (void *)convolucion, &threadId[i]);
    
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }
  
  imwrite("blur.png", blurImage);
  free(gauss);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", blurImage );
  cv::waitKey(0);
  return 0;
}