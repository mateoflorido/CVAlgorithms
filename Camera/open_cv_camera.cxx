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
 
    BoundariesIn[0] = Point2f( -30,-60 );
    BoundariesIn[1] = Point2f( image.cols+50,-50);
    BoundariesIn[2] = Point2f( image.cols+100,image.rows+50);
    BoundariesIn[3] = Point2f( -50,image.rows+50  );  
    BoundariesOut[0] = Point2f( 0,0 );
    BoundariesOut[1] = Point2f( image.cols-1,0);
    BoundariesOut[2] = Point2f( image.cols-1,image.rows-1);
    BoundariesOut[3] = Point2f( 0,image.rows-1  );
  warpMat = getPerspectiveTransform(BoundariesIn, BoundariesOut);

  
  dst_image = Mat::zeros(image.rows, image.cols, image.type());
  warpPerspective(image, dst_image, warpMat, image.size());  
  //Write something
  imshow("Image",image);
  imshow("Image Warp",dst_image);
  waitKey(0);
  imwrite( basename + "_warp.png", dst_image);
  return( 0 );
}

// eof - example.cxx
