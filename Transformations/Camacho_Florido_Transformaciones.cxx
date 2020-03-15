#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;


/** @function main */
 int main( int argc, char** argv )
 {
  
  // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  Mat src;
  src = imread( argv[1], 1 );

  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  //imshow("Original", src);
   //Create each transform matrix
   //Translation Matrix
    Mat Mt(2,3,CV_64F, cvScalar(0.0));
    Mt.at<double>(0,0) = 1;    
    Mt.at<double>(1,1) = 1;
    Mt.at<double>(1,0) = 0; 
    Mt.at<double>(0,1) = 0;    
    Mt.at<double>(1,2) = -37;
    Mt.at<double>(0,2) = -42;
    
    //Rotation Matrix
    Mat Mr(2,3,CV_64F, cvScalar(0.0));
    Mr.at<double>(0,0) = cos(0.610865);    //35 degrees = 0.610865 radians
    Mr.at<double>(1,1) = cos(0.610865);
    Mr.at<double>(1,0) = sin(0.610865); 
    Mr.at<double>(0,1) = -sin(0.610865);    
    Mr.at<double>(1,2) = 1;
    Mr.at<double>(0,2) = 1;

    //Scale Matrix
    Mat Ms(2,3,CV_64F, cvScalar(0.0));
    Ms.at<double>(0,0) = 0.8;    
    Ms.at<double>(1,1) = 0.8;
    Ms.at<double>(1,0) = 0; 
    Ms.at<double>(0,1) = 0;    
    Ms.at<double>(1,2) = 0;
    Ms.at<double>(0,2) = 0;
    
    //Matrix Multiplication
   
    //Ms.Mr.Mt
    Mat Msrt(2,3,CV_64F, cvScalar(0.0));
    Msrt.at<double>(0,0) = 0.655322;    
    Msrt.at<double>(1,1) = 0.655322;
    Msrt.at<double>(1,0) = 0.458861; 
    Msrt.at<double>(0,1) = -0.458861;    
    Msrt.at<double>(1,2) = -42.7191;
    Msrt.at<double>(0,2) = -9.74565;

    //Write and show Test 1
    Mat resul1=Mat::zeros(src.rows,src.cols,src.type());
    warpAffine(src, resul1, Msrt, src.size());
    //imshow("Test 1",resul1);
    imwrite("Test1.png", resul1);
    

    //Test 2, each operation separated

    
    //Moved
    Mat dstTrans;
    dstTrans=Mat::zeros(src.rows,src.cols,src.type());
    warpAffine(src,dstTrans,Mt,src.size());
    //imshow("Transladada",dstTrans);
    
    //Rotation
    Mat dstRot;
    dstRot=Mat::zeros(src.rows,src.cols,src.type());
    warpAffine(src,dstRot,Mr,src.size());
    //imshow("Rotada",dstRot);

    //Scalling
    Mat dstScal;
    dstScal=Mat::zeros(dstRot.rows,dstRot.cols,dstRot.type());
    warpAffine(dstRot, dstScal, Ms, dstRot.size());
    //imshow("Escalada",dstScal);
    imwrite("Test2.png", dstScal);
    waitKey(0);
    return 0;
  } 