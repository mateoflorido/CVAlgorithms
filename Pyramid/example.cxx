#include <iostream>
#include <iterator>
#include <opencv2/opencv.hpp>
//
#define ONESIXTEEN 0.0625f;
#define ONEEIGTH 0.125f;
#define ONEFOURTH 0.25f;
#define ONE 1.0f;
#define TWENTYFOURSIXTEEN 1.5f;
#define THRITYSIXSIXTEEN 2.25f;


using namespace cv;

void DownSamplingImage( Mat &m_OriginalImage, Mat &m_SampledImage )
{
  int col = 0;
  int row = 0;
  int halt;
  int m_Size = m_SampledImage.size( ).width;
  std::cout << "Creating an Image of " << m_Size << " x " << m_Size << std::endl;
  MatIterator_< Vec3b > it, end, it_SS, end_SS;

  it = m_OriginalImage.begin< Vec3b >( );
  end = m_OriginalImage.end< Vec3b >( );
  it_SS = m_SampledImage.begin< Vec3b >( );
  end_SS = m_SampledImage.end< Vec3b >( );

  while( it != end && it_SS != end_SS )
  {
    if( col > m_Size )
    {
      col = 0;
      row++;
    }
    if( row % 2 == 0 )
    {
      std::cout << "Skipping row #" << row << " To: "<< m_Size<< std::endl;
      std::next( it, m_Size );
      col = 0;
      row++;
      continue;
    }
    else
    {
      it++;
    }
    if( col % 2 != 0)
    {
      (*it_SS)[ 0 ] = (*it)[ 0 ];
  	  (*it_SS)[ 1 ] = (*it)[ 1 ];
  	  (*it_SS)[ 2 ] = (*it)[ 2 ];
      it_SS++;
    }
    col++;
  }
/*
  for(  ; it != end, it_SS != end_SS; ++it )
  {
    if( col >  m_Size )
    {
      col = 0;
      row++;
    }
    if( ( col % 2 != 0 ) && ( row % 2 != 0 ) )
    {
      (*it_SS)[ 0 ] = (*it)[ 0 ];
  	  (*it_SS)[ 1 ] = (*it)[ 1 ];
  	  (*it_SS)[ 2 ] = (*it)[ 2 ];
      it_SS++;
    }
    col ++;
  } // rof*/
}


void PyramidDown( Mat &m_OriginalImage, Mat &m_GaussianKernel, std::string basename )
{
  int i = 3;
  Mat m_TreatmentImage = m_OriginalImage;
  while( i > 0)
  {
    std::cout << "Creating " + std::to_string( i ) << " image.\n";
    Mat m_MiddleImage = Mat::zeros( m_TreatmentImage.size( ).width, m_TreatmentImage.size( ).height, CV_8UC3);
    GaussianBlur( m_OriginalImage, m_MiddleImage, m_GaussianKernel.size( ), 0.85d );
    imwrite( basename + "_GaussianBlurNormal" + std::to_string( i ) + ".png", m_MiddleImage );
    Mat m_DownSampled = Mat::zeros( m_TreatmentImage.size( ).width / 2, m_TreatmentImage.size( ).height / 2, CV_8UC3);
    DownSamplingImage( m_MiddleImage, m_DownSampled );
    imwrite( basename + "_GaussianBlur" + std::to_string( i ) + ".png", m_DownSampled );
    m_TreatmentImage = m_DownSampled;
    i--;

  }
  
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

   // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

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
  

  

  //filter2D( rImg, m_BlurRedC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( gImg, m_BlurGreenC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( bImg, m_BlurBlueC, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //filter2D( image, m_TestImgBlr, m_DDepth, m_GaussianKernel, m_Anchor, m_Delta, BORDER_DEFAULT );
  //First Blur filter !!!!!!!!!!!!!!!!!!!!!!!
  GaussianBlur( image, m_TestImgBlr, m_GaussianKernel.size( ), 0.85d );
  PyramidDown( image, m_GaussianKernel, basename );
  
  //First Laplacian Matrix UP !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Declare the Laplacian Matrix
  Mat m_Laplaciana1;
  m_Laplaciana1= Mat::zeros( image.size( ), CV_8UC3);
  //Declare the iterators for the blurred matrix, the laplacian matrix and the original
  MatIterator_< Vec3b > it_BL, end_BL, it_LP, end_LP, it, end ;
  //Initialize iterators 
  it_BL = m_TestImgBlr.begin < Vec3b >();
  end_BL = m_TestImgBlr.end < Vec3b >();
  it_LP = m_Laplaciana1.begin < Vec3b >();
  end_LP = m_Laplaciana1.end < Vec3b >();
  it = image.begin < Vec3b >();
  end = image.end < Vec3b >();
  //Laplacian Matrix = Original Matrix - Blurred Matrix 
  for( ;it!= end, it_BL!= end_BL, it_LP!= end_LP; it++, it_BL++, it_LP++){
    (*it_LP)[0]=(*it)[0]-(*it_BL)[0];
    (*it_LP)[1]=(*it)[1]-(*it_BL)[1];
    (*it_LP)[2]=(*it)[2]-(*it_BL)[2];
  } // rof

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
  //First SubSample UP!!!!!!!!!!!!!!!
  MatIterator_< Vec3b > it_SS, end_SS;
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
  // First Sampling Down (más grande)!!!!!!!!!!!!!!!!!!!!!!!
  //Declare Sampling Down Matrix (2048x2048)
  Mat m_TestImgDS1 = Mat::zeros( image.size().width * 2 ,image.size().height * 2, CV_8UC3);
  //Declare UpSampe Iterators
  MatIterator_< Vec3b > it_DS, end_DS;
  it=image.begin < Vec3b >();
  end=image.end < Vec3b >();
  it_DS=m_TestImgDS1.begin < Vec3b >();
  end_DS=m_TestImgDS1.end < Vec3b >();
  //Fill the even rows and cols with 0
  col=0;
  row=0;
  for(  ; it != end, it_DS != end_DS; ++it_DS){
      if(col>2047){
        row++;
        col=0;
      } 
      if((col%2!=0)&&(row%2!=0)){
        (*it_DS)[0]=(*it)[0];
        (*it_DS)[1]=(*it)[1];
        (*it_DS)[2]=(*it)[2];
        it++;
      }else{
        (*it_DS)[0]=0;
        (*it_DS)[1]=0;
        (*it_DS)[2]=0;
      }
      col++;
  } //rof


  //GaussianBlur Down 1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Declare GaussianBlur Down Matrix (2048x2048)
  Mat m_TestImgDBl1;
  m_TestImgDBl1= Mat::zeros( m_TestImgDS1.size( ), CV_8UC3);
  //Call convolution function
  GaussianBlur( m_TestImgDS1, m_TestImgDBl1, m_GaussianKernel.size( ), 0.85d );

  //Laplacian Down 1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Declare first Down Laplacian Matrix (2048x2048)
  Mat m_DLaplacian1;
  m_DLaplacian1 = Mat::zeros( m_TestImgDS1.size( ), CV_8UC3);
  //Declare iterators
  MatIterator_< Vec3b > it_DLP, end_DLP, it_DBL, end_DBL;
  it_DS = m_TestImgDS1.begin< Vec3b >();
  end_DS = m_TestImgDS1.end< Vec3b >();
  it_DLP = m_DLaplacian1.begin< Vec3b >();
  end_DLP = m_DLaplacian1.end< Vec3b >();
  it_DBL = m_TestImgDBl1.begin< Vec3b >();
  end_DBL = m_TestImgDBl1.end< Vec3b >();
  //Down Laplacian Matrix = First Down Sampled Matrix - GaussianBlur Down Matrix 
  for( ;it_DS!= end_DS, it_DBL!= end_DBL, it_DLP!= end_DLP; it_DS++, it_DBL++, it_DLP++){
    (*it_DLP)[0]=(*it_DS)[0]-(*it_DBL)[0];
    (*it_DLP)[1]=(*it_DS)[1]-(*it_DBL)[1];
    (*it_DLP)[2]=(*it_DS)[2]-(*it_DBL)[2];
  } // rof
   

 

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

  imwrite( basename + "_GAussianSS1.png", m_TestImgSS1 ); //Up Sample 1
  imwrite( basename + "_GAussian1.png", m_TestImgBlr ); //Up Blur 1
  imwrite( basename + "_Laplacian1.png", m_Laplaciana1); //UpLaplacian 1
  imwrite( basename + "_OrigninalDS1.png", m_TestImgDS1); //DownSample 1
  imwrite( basename + "_GaussianD1.png", m_TestImgDBl1); // Blur DownSample 1
  imwrite( basename + "_DLaplacian1.png", m_DLaplacian1); // DownLaplacian 1
  return( 0 );
}

// eof - 01_color_channels.cxx
