#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;


void CannyThreshold(int Thresh, Mat& src_gray, Mat& detected_edges, int ratio, int kernel_size, Mat& dst, Mat& src)
{
    //Blur Gray Scale Image
    blur( src_gray, detected_edges, Size(3,3) );
    //Canny Method
    Canny( detected_edges, detected_edges, Thresh, Thresh*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
}

int main(int argc, char** argv )
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

  //Save basename 
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename ,'.' );
  // Read an image
  Mat image;
  image = imread( argv[1], 1 );

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi

    //Canny Method Code

  Mat src, src_gray;
  Mat dst, detected_edges;
  src=image.clone();
  dst.create( image.size(), image.type() );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  //Use Otsu's Method to find Threshold
  int thresh = threshold(src_gray, dst,0,255,CV_THRESH_OTSU)*0.4;
  std::cout<<thresh<<std::endl;
  CannyThreshold(thresh, src_gray, detected_edges, 3,3, dst, src);

  //Back Projection Method Code
  //Transform to hsvMat src, src_gray;
  Mat hsv;
  cvtColor( image, hsv, COLOR_BGR2HSV );

  //Find required Mask
  Point seed = Point( 0, 0 );

  int newMaskVal = 255;
  Scalar newVal = Scalar( 120, 120, 120 );

  int connectivity = 8;
  int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

  Mat mask2 = Mat::zeros( image.rows + 2, image.cols + 2, CV_8U );
  floodFill( image, mask2, seed, newVal, 0, Scalar( 20, 20, 20 ), Scalar( 20, 20, 20), flags );
  Mat mask = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );

  //Initialize Histogram
  Mat hist;
  int h_bins = 30; int s_bins = 32;
  int histSize[] = { h_bins, s_bins };
  float h_range[] = { 0, 180 };
  float s_range[] = { 0, 256 };
  const float* ranges[] = { h_range, s_range };

  int channels[] = { 0, 1 };

  /// Get the Histogram and normalize it
  calcHist( &hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false );
  //calcHist( &hsv, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
  normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

  /// Get Backprojection
  Mat backproj;
  calcBackProject( &hsv, 1, channels, hist, backproj, ranges, 1, true );
  
  //Add Canny and Back Projection Segmentations
  Mat suma; 
  add(backproj,detected_edges, suma);
  
  //Write Canny's segementation, Back Projection segmentation and added segmentation
  imwrite( basename + "_cannySeg.png", detected_edges);
  imwrite( basename + "_backProjSeg.png", backproj);
  imwrite( basename + "_finalSeg.png", suma);
  return( 0 );
}

// eof - example.cxx
