#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Input Quadilateral or Image plane coordinates
    Point2f BoundariesIn[4]; 
    // Output Quadilateral or World plane coordinates
    Point2f BoundariesOut[4];
  std::string basename = argv[ 1 ];
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
  Mat image;
  Mat dst_image;
  Mat warpMat(3,3, CV_64F, cvScalar(0.0));

  image = imread( argv[1], 1 );
  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi


  warpMat.at<double>(0,0) = 630.7903;
  warpMat.at<double>(0,1) = 0;
  warpMat.at<double>(0,2) = 645.7903;
  warpMat.at<double>(1,0) = 0;
  warpMat.at<double>(1,1) = 630.7903;
  warpMat.at<double>(1,2) = 366.7903;
  warpMat.at<double>(2,0) = 0;
  warpMat.at<double>(2,1) = 0;
  warpMat.at<double>(2,2) = 1;

  
  dst_image = Mat::zeros(image.rows, image.cols, image.type());
  undistort(image, dst_image, warpMat, image.size());  
  //Write something
  imshow("Image",image);
  imshow("Image Dist",dst_image);
  waitKey(0);
  imwrite( basename + "_disto.png", dst_image);
  return( 0 );
}

// eof - example.cxx
