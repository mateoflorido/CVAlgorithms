#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

/// Global variables
char* source_window = "Source image";
char* warp_window = "Warp";
char* warp_rotate_window = "Warp + Rotate";

/** @function main */
 int main( int argc, char** argv )
 {
   Mat warp_dest;
   Mat src;

   /// Load the image
   src = imread( argv[1], 1 );
 
   //Matriz de translación
    Mat Mt(2,3,CV_64F, cvScalar(0.0));

    Mt.at<double>(0,0) = 1;    
    Mt.at<double>(1,1) = 1;
    Mt.at<double>(1,0) = 0; 
    Mt.at<double>(0,1) = 0;    
    Mt.at<double>(1,2) = 37;
    Mt.at<double>(0,2) = -42;
    
    //Matriz de rotación
    Mat Mr(2,3,CV_64F, cvScalar(0.0));

    Mr.at<double>(0,0) = cos(45);    
    Mr.at<double>(1,1) = cos(45);
    Mr.at<double>(1,0) = sin(45); 
    Mr.at<double>(0,1) = -sin(45);    
    Mr.at<double>(1,2) = 1;
    Mr.at<double>(0,2) = 1;

    //Matriz de escalamiento
    Mat Me(2,3,CV_64F, cvScalar(0.0));

    Me.at<double>(0,0) = 0.8;    
    Me.at<double>(1,1) = 0.8;
    Me.at<double>(1,0) = 0; 
    Me.at<double>(0,1) = 0;    
    Me.at<double>(1,2) = 0;
    Me.at<double>(0,2) = 0;
    
    //Multiplicación de matrices
    Mat Mer= Me.mul(Mr);
    Mer = Mer.mul(Mt);

    //Prueba con Matriz resultante
    Mat resulA=Mat::zeros(src.rows,src.cols,src.type());
    warpAffine(src, resulA, Mer, src.size());
    imshow("A mano",resulA);
    cv::Mat dest;
    cv::Size size(src.cols,src.rows);
    warpAffine(src, dest, Mt, size);
    
    
    warp_dest=Mat::zeros(src.rows,src.cols,src.type());
    Point center=Point(warp_dest.cols/2,warp_dest.rows/2);
    double angle=-35;
    Mat warp_dest_rot;
    Mat rot_mat = getRotationMatrix2D( center, angle, 0.8);
    warpAffine(dest, warp_dest_rot, rot_mat,warp_dest.size());
    imshow("Rotada",warp_dest_rot);
    waitKey(0);
    return 0;
  }