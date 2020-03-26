
/*                     License-Identifier: MIT License                        */
/******************************************************************************
 *
 * Name: otsu.cxx - OpenCV Otsu Binarization Algorithm
 * 
 * Authors: Nicolas Camacho Plazas nicolas-camacho@javeriana.edu.co
 *          Mateo Florido Sanchez  floridom@javeriana.edu.co
 *
 *
 *****************************************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

float Mean(const Mat &m_Histogram, int m_Start, int m_End )
{
  float mean = 0;
  float sum = 0;

  for( int i = m_Start; i < m_End; i++ )
  {
    mean += i * m_Histogram.at<float>(i);
    sum += m_Histogram.at<float>(i);
  }

  if( mean == 0 && sum == 0)
    return 0;
  return ( mean / sum );

}

float Weight( const Mat &m_Histogram, int m_Start, int m_End, int m_PixelCount )
{
  float sum = 0;

  for( int i = m_Start; i < m_End; i++ )
  {
    sum += m_Histogram.at<float>(i);
  }

  return ( sum / m_PixelCount ); 
}

float Variance( const Mat &m_Histogram, float mean, int m_Start, int m_End )
{
  float variance = 0;
  float sum = 0;

  for( float i = m_Start; i < m_End; i+=1.0f )
  {
    variance += ( i - mean ) * ( i - mean ) * m_Histogram.at<float>(i);
    sum += m_Histogram.at<float>(i);
  }
  return ( variance / sum );
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
  Mat src;
  src = imread( argv[1], 1 );

  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  
  //Save basename 
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename ,'.' );

  Mat image_gray, ourThreshold, otsuThreshold;

  cvtColor( src, image_gray, COLOR_RGB2GRAY );

  /// Establish the number of slots
  int histSize = 256;

  /// Set the ranges
  float range[] = { 0, 256 } ;
  const float* histRange = { range };
  bool uniform = true; 
  bool accumulate = false;

  Mat gray_hist;

  /// Extract the Histograms
  calcHist( &image_gray, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate );
  // Normalize the Histogram
  normalize( gray_hist, gray_hist, 0, gray_hist.rows, NORM_MINMAX, -1, Mat( ) );

  float minwCV = std::numeric_limits<float>::infinity( );
  int thresh = -1;
  float weight1 = 0;
  float weight2 = 0;
  float variance1 = 0;
  float variance2 = 0;
  float mean1 = 0;
  float mean2 = 0;
  float m_PixelCount = src.size( ).width * src.size( ).height;
  float wCV = 0;

  for( int i = 1 ; i < histSize; i++ )
  {
    mean1 = Mean( gray_hist, 0, i);
    mean2 = Mean( gray_hist, i, histSize );
    weight1 = Weight( gray_hist, 0, i, m_PixelCount );
    weight2 = Weight( gray_hist, i, histSize, m_PixelCount );
    variance1 = Variance( gray_hist, mean1, 0, i );
    variance2 = Variance( gray_hist, mean2, i, histSize );
    wCV = weight1 * variance1 + weight2 * variance2;
    if( wCV < minwCV )
    {
      minwCV = wCV;
      thresh = i;
    }
  }

  threshold( image_gray, ourThreshold, thresh, histSize - 1, THRESH_BINARY );
  //threshold( image_gray, otsuThreshold, thresh, histSize - 1, THRESH_OTSU );

  imwrite( basename + "_Threshold.png", ourThreshold );
  //imwrite( basename + "_otsuThreshold.png", otsuThreshold );

  return( 0 );

}
// eof - otsu.cxx