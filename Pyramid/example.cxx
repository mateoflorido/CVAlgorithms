#include <stdio.h>
#include <opencv2/opencv.hpp>
#define ONESIXTEEN 0.0625f;
#define ONEEIGTH 0.125f;
#define ONEFOURTH 0.25f;
#define ONE 1.0f;
#define TWENTYFOURSIXTEEN 1.5f;
#define THRITYSIXSIXTEEN 2.25f;


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
  Mat m_DestOriginal;
  Mat m_BlurRedC;
  Mat m_BlurGreenC;
  Mat m_BlurBlueC;
  Mat m_BlurRedSS1;
  Mat m_BlurBlueSS1;
  Mat m_BlurGreenSS1;

  image = imread( argv[1], 1 );

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi

  // Create color channel images
  // red channel
  Mat rImg = Mat::zeros( image.size( ), CV_8UC3 );
  Mat m_TestImgBlr = Mat::zeros( image.size( ), CV_8UC3);
  Mat m_TestImgSS1 = Mat::zeros( image.size().width / 2 ,image.size().height / 2, CV_8UC3);
  m_BlurRedC = Mat::zeros( image.size( ), CV_8UC3 );
  m_BlurRedSS1 = Mat::zeros( image.size().width / 2 ,image.size().height / 2, CV_8UC3 );
  //std::cout << "Subsample Size: w:" << image.size( ).width / 2 << " h:" << image.size( ).height / 2 << std::endl;
  // green channel
  Mat gImg = Mat::zeros( image.size( ), CV_8UC3 );
  m_BlurGreenC = Mat::zeros( image.size( ), CV_8UC3 );
  m_BlurGreenSS1 = Mat::zeros( image.size().width / 2 ,image.size().height / 2, CV_8UC3 );
  // blue channel
  Mat bImg = Mat::zeros( image.size( ), CV_8UC3 );
  m_BlurBlueC = Mat::zeros( image.size( ), CV_8UC3 );
  m_BlurBlueSS1 = Mat::zeros( image.size().width / 2 ,image.size().height / 2, CV_8UC3 );
  // composite image (RGB)
  Mat rgbImg = Mat::zeros( image.size( ), CV_8UC3 );
   
  // Fill color channel images
  /*MatIterator_< Vec3b > it, crIt, cgIt, cbIt, rgbIt, end, endr, endg, endb, endrgb;
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

  } // rof*/

  //Create Gaussian Kernel
  Mat m_GaussianKernel;
  int m_KernelSize = 3;
  int m_DDepth = -1;
  int m_Delta = 0;
  Point m_Anchor;
  m_Anchor = Point( 1, 1 );
  m_GaussianKernel = Mat::ones( m_KernelSize, m_KernelSize, CV_8UC3 );
  m_GaussianKernel.at<float>(0,0) = ONESIXTEEN;
  m_GaussianKernel.at<float>(0,1) = ONEEIGTH;
  m_GaussianKernel.at<float>(0,2) = ONESIXTEEN;
  m_GaussianKernel.at<float>(1,0) = ONEEIGTH;
  m_GaussianKernel.at<float>(1,1) = ONEFOURTH;
  m_GaussianKernel.at<float>(1,2) = ONEEIGTH;
  m_GaussianKernel.at<float>(2,0) = ONESIXTEEN;
  m_GaussianKernel.at<float>(2,1) = ONEEIGTH;
  m_GaussianKernel.at<float>(2,2) = ONESIXTEEN;
  

  //First Blur filter

  //filter2D( rImg, m_BlurRedC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( gImg, m_BlurGreenC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( bImg, m_BlurBlueC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( image, m_TestImgBlr, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  GaussianBlur( image, m_TestImgBlr, m_GaussianKernel.size( ), 0.85d );
  /*
  MatIterator_< Vec3b > it, crIt, cgIt, cbIt, rgbIt, end, endr, endg, endb, endrgb;
  it = m_TestImgBlr.begin< Vec3b >( );
  crIt = m_BlurRedC.begin< Vec3b >( );
  cgIt = m_BlurGreenC.begin< Vec3b >( );
  cbIt = m_BlurBlueC.begin< Vec3b >( );
  end = m_TestImgBlr.end< Vec3b >( );
  endr = m_BlurRedC.end< Vec3b >( );
  endg = m_BlurGreenC.end< Vec3b >( );
  endb = m_BlurBlueC.end< Vec3b >( );
  int cont =0;
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
    cont ++;

  } // rof*/
  MatIterator_< Vec3b > it, end, it_SS, end_SS;
  it = m_TestImgBlr.begin< Vec3b >( );
  end = m_TestImgBlr.end< Vec3b >( );
  it_SS = m_TestImgSS1.begin< Vec3b >( );
  end_SS = m_TestImgSS1.end< Vec3b >( );
  int col =0;
  int row =0;
  for(  ; it != end, it_SS != end_SS; ++it)
  {
    if(col>1023){
      row ++;
      col=0;
    }
    if((col%2!=0)&&(row%2!=0)){
      (*it_SS)[0] = (*it)[0];
  	  (*it_SS)[1] = (*it)[1];
  	  (*it_SS)[2] = (*it)[2];
      it_SS++;
    }
    col ++;
  } // rof
  std::cout<<row<<std::endl;
  //First SubSample

  
  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );


  /*imwrite( basename + "_R.png", rImg );
  imwrite( basename + "_G.png", gImg );
  imwrite( basename + "_B.png", bImg );
  
  imwrite( basename + "_GaussianR.png", m_BlurRedC );
  imwrite( basename + "_GAussianG.png", m_BlurGreenC );
  imwrite( basename + "_GAussianB.png", m_BlurBlueC );
  

  imwrite( basename + "_GaussianRSS1.png", m_BlurRedSS1 );
  imwrite( basename + "_GAussianGSS1.png", m_BlurGreenSS1 );
  imwrite( basename + "_GAussianBSS1.png", m_BlurBlueSS1 );
  */
  imwrite( basename + "_GAussianTestSS1.png", m_TestImgSS1 );
  imwrite( basename + "_GAussianTest.png", m_TestImgBlr );
  
  

  return( 0 );
}

// eof - 01_color_channels.cxx
