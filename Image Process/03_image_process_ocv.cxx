#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

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

  // Create color channel images
  // red channel
  Mat rImg = Mat::zeros( image.size( ), CV_8UC3 );
  // green channel
  Mat gImg = Mat::zeros( image.size( ), CV_8UC3 );
  // blue channel
  Mat bImg = Mat::zeros( image.size( ), CV_8UC3 );
  // composite image (RGB)
  Mat rgbImg = Mat::zeros( image.size( ), CV_8UC3 );
   
  // Fill color channel images
  MatIterator_< Vec3b > it, crIt, cgIt, cbIt, rgbIt, end, endr, endg, endb, endrgb;
  it = image.begin< Vec3b >( );
  crIt = rImg.begin< Vec3b >( );
  cgIt = gImg.begin< Vec3b >( );
  cbIt = bImg.begin< Vec3b >( );
  end = image.end< Vec3b >( );
  endr = rImg.end< Vec3b >( );
  endg = gImg.end< Vec3b >( );
  endb = bImg.end< Vec3b >( );
  for(  ; it != end, crIt != endr, cgIt != endg, cbIt != endb; ++it, ++crIt, ++cgIt, ++cbIt)
  {
  	(*crIt)[0] = 0;
  	(*crIt)[1] = 0;
  	(*crIt)[2] = (*it)[2];

  	(*cgIt)[0] = 0;
  	(*cgIt)[1] = (*it)[1];
  	(*cgIt)[2] = 0;
  	
  	(*cbIt)[0] = (*it)[0];
  	(*cbIt)[1] = 0;
  	(*cbIt)[2] = 0;
  } // rof

  // From color channel images, reconstruct the original color image
  crIt = rImg.begin< Vec3b >( );
  cgIt = gImg.begin< Vec3b >( );
  cbIt = bImg.begin< Vec3b >( );
  rgbIt = rgbImg.begin< Vec3b >( );
  endr = rImg.end< Vec3b >( );
  endg = gImg.end< Vec3b >( );
  endb = bImg.end< Vec3b >( );
  endrgb = rgbImg.end< Vec3b >( );
  for(  ; crIt != endr, cgIt != endg, cbIt != endb, rgbIt != endrgb; ++crIt, ++cgIt, ++cbIt, ++rgbIt)
  {
  	(*rgbIt)[0] = (*cbIt)[0];
  	(*rgbIt)[1] = (*cgIt)[1];
  	(*rgbIt)[2] = (*crIt)[2];
  	
  } // rof

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_R.png", rImg );
  imwrite( basename + "_G.png", gImg );
  imwrite( basename + "_B.png", bImg );

  //Rescale each channel

  Mat rsRImg;
  Mat rsGImg;
  Mat rsBImg;

  resize(rImg, rsRImg, Size(), atof( "0.75" ), atof( "0.75" ), INTER_LINEAR);
  resize(gImg, rsGImg, Size(), atof( "0.5" ), atof( "0.5" ), INTER_LINEAR);
  resize(bImg, rsBImg, Size(), atof( "0.25" ), atof( "0.25" ), INTER_LINEAR);

  //Write rescaled channels
  imwrite( basename + "_sR.png", rsRImg );
  imwrite( basename + "_sG.png", rsGImg );
  imwrite( basename + "_sB.png", rsBImg );

  //Expand each channel
  Mat exRImg;
  Mat exGImg;
  Mat exBImg;

  resize(rsRImg, exRImg, Size(), 1.3333333333333333, 1.3333333333333333, INTER_LINEAR);
  resize(rsGImg, exGImg, Size(), 2, 2, INTER_LINEAR);
  resize(rsBImg, exBImg, Size(), 4, 4, INTER_LINEAR);

  //Write expanded channels
  imwrite( basename + "_ssR.png", exRImg );
  imwrite( basename + "_ssG.png", exGImg );
  imwrite( basename + "_ssB.png", exBImg );
  
  // From color channel expanded images, reconstruct an RGB image
  
  Mat resRGB=rgbImg;
  crIt = exRImg.begin< Vec3b >( );
  cgIt = exGImg.begin< Vec3b >( );
  cbIt = exBImg.begin< Vec3b >( );
  rgbIt = resRGB.begin< Vec3b >( );
  endr = exRImg.end< Vec3b >( );
  endg = exGImg.end< Vec3b >( );
  endb = exBImg.end< Vec3b >( );
  endrgb = resRGB.end< Vec3b >( );
  for(  ; crIt != endr, cgIt != endg, cbIt != endb, rgbIt != endrgb; ++crIt, ++cgIt, ++cbIt, ++rgbIt)
  {
  	(*rgbIt)[0] = (*cbIt)[0];
  	(*rgbIt)[1] = (*cgIt)[1];
  	(*rgbIt)[2] = (*crIt)[2];
  	
  } // rof

  //Write results
  imwrite( basename + "_rRGB.png", resRGB);
  // Compare the original and the expanded RGB image
  Mat diffImg=rgbImg;
  MatIterator_< Vec3b > erIt, egIt, ebIt, endEr, endEg, endEb, diffIt, endDiff;
  crIt = rImg.begin< Vec3b >( );
  cgIt = gImg.begin< Vec3b >( );
  cbIt = bImg.begin< Vec3b >( );
  endr = rImg.end< Vec3b >( );
  endg = gImg.end< Vec3b >( );
  endb = bImg.end< Vec3b >( );
  
  erIt = exRImg.begin< Vec3b >( );
  egIt = exGImg.begin< Vec3b >( );
  ebIt = exBImg.begin< Vec3b >( );
  endEr = exRImg.end< Vec3b >( );
  endEg = exGImg.end< Vec3b >( );
  endEb = exBImg.end< Vec3b >( );

  diffIt = resRGB.begin< Vec3b >( );
  endDiff = resRGB.end< Vec3b >( );
  
  for(  ; diffIt != endDiff, erIt != endEr, egIt != endEg, ebIt != endEb, crIt != endr, cgIt != endg, cbIt != endb 
  ; ++diffIt, ++crIt, ++cgIt, ++cbIt, ++erIt, ++egIt, ++ebIt)
  {
    (*diffIt)[0] = (*cbIt)[0]-(*ebIt)[0];
  	(*diffIt)[1] = (*cgIt)[1]-(*egIt)[1];
  	(*diffIt)[2] = (*crIt)[2]-(*erIt)[2];
  } // rof
  
  //Write differences found
  imwrite( basename + "_diff.png", diffImg);
  return( 0 );
}

// eof - 03_image_process_ocv.cxx