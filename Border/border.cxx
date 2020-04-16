#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

const int ratio = 3;
const int kernel_size = 3;

void CannyThreshold(int Thresh, Mat& src_gray, Mat& detected_edges, int ratio, int kernel_size, Mat& dst, Mat& src)
{
    blur( src_gray, detected_edges, Size(3,3) );
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

  // Read an image
  Mat image;
  image = imread( argv[1], 1 );

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
    //Canny Code
  Mat src, src_gray;
  Mat dst, detected_edges;
  src=image.clone();
  dst.create( image.size(), image.type() );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  CannyThreshold(threshold(src_gray, dst,0,255,CV_THRESH_OTSU), src_gray, detected_edges, 3,3, dst, src);
  //Transform to hsvMat src, src_gray;
  Mat hsv;
  cvtColor( image, hsv, COLOR_BGR2HSV );
   Point seed = Point( 0, 0 );

    int newMaskVal = 255;
    Scalar newVal = Scalar( 120, 120, 120 );

    int connectivity = 8;
    int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

    Mat mask2 = Mat::zeros( image.rows + 2, image.cols + 2, CV_8U );
    floodFill( image, mask2, seed, newVal, 0, Scalar( 20, 20, 20 ), Scalar( 20, 20, 20), flags );
    Mat mask = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );

 
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

  //Save basename 
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename ,'.' );

  Mat suma; 
  add(backproj,detected_edges, suma);
  
  //Write something
  imwrite( basename + "_example.png", suma);
  return( 0 );
}

// eof - example.cxx
