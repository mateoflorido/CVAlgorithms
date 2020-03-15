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

void PyramidDown( Mat &m_WorkingImage, Mat &m_CurrentImage, Mat &m_GaussianKernel, std::string m_Basename, int m_Identifier )
{
  bool isSummit = false;
  Mat m_BlurredImage = Mat::zeros( m_WorkingImage.size( ), CV_8UC3 );
  Mat m_UpSampled = Mat::zeros( m_WorkingImage.size().width / 2, m_WorkingImage.size().height / 2, CV_8UC3 );

  GaussianBlur( m_WorkingImage, m_BlurredImage, m_GaussianKernel.size( ), 0.85d );
      
  MatIterator_< Vec3b > it_BL, end_BL, it_LP, end_LP, it, end ;

  if( m_Identifier != 1 )
  {
    //Declare the Laplacian Matrix
    Mat m_Laplacian = Mat::zeros( m_WorkingImage.size( ), CV_8UC3 );
    //Declare the iterators for the blurred matrix, the laplacian matrix and the original
    //Initialize iterators 
    it_BL   = m_BlurredImage.begin < Vec3b >( );
    end_BL  = m_BlurredImage.end < Vec3b >( );
    it_LP   = m_Laplacian.begin < Vec3b >( );
    end_LP  = m_Laplacian.end < Vec3b >( );
    it      = m_WorkingImage.begin < Vec3b >( );
    end     = m_WorkingImage.end < Vec3b >( );
    //Laplacian Matrix = Original Matrix - Blurred Matrix 
    for( ;it!= end, it_BL!= end_BL, it_LP!= end_LP; it++, it_BL++, it_LP++){
      (*it_LP)[ 0 ]=(*it)[0] - (*it_BL)[0];
      (*it_LP)[ 1 ]=(*it)[1] - (*it_BL)[1];
      (*it_LP)[ 2 ]=(*it)[2] - (*it_BL)[2];
    } // rof
    imwrite( m_Basename + "_Laplacian" + std::to_string(m_Identifier) + ".png", m_Laplacian);
  
  }
  else
  {
    std::cout<<"Summit!\n";
    std::cout << "Size: " << m_WorkingImage.size().width << std::endl;
    isSummit = true;
  }
    
  MatIterator_< Vec3b > it_SS, end_SS;
  it = m_BlurredImage.begin< Vec3b >( );
  end = m_BlurredImage.end< Vec3b >( );
  it_SS = m_UpSampled.begin< Vec3b >( );
  end_SS = m_UpSampled.end< Vec3b >( );
  int col =0;
  int row =0;

  for(  ; it != end, it_SS != end_SS; ++it)
  {
    if(col > m_WorkingImage.size().width - 1 ){
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
  if( isSummit )
  {
    imwrite( m_Basename + "_Laplacian" + std::to_string(m_Identifier) + ".png", m_UpSampled);
  }
  imwrite( m_Basename + "_Gaussian" + std::to_string(m_Identifier) + ".png", m_UpSampled);
    


  

  m_CurrentImage = m_UpSampled.clone( );
}

void PyramidUp( Mat &m_WorkingImage, Mat &m_CurrentImage, Mat &m_GaussianKernel, std::string m_Basename, int m_Identifier )
{
  int col = 0;
  int row = 0;
  // Declare Sampling Down Matrix (Double Size)
  Mat m_DownSampled = Mat::zeros( m_WorkingImage.size().width * 2 ,m_WorkingImage.size().height * 2, CV_8UC3);

  // Declare UpSample Iterators
  MatIterator_< Vec3b > it, end, it_DS, end_DS;
  it     = m_WorkingImage.begin < Vec3b >();
  end    = m_WorkingImage.end < Vec3b >();
  it_DS  = m_DownSampled.begin < Vec3b >();
  end_DS = m_DownSampled.end < Vec3b >();
  
  // Fill the even rows and cols with 0
  for(  ; it != end, it_DS != end_DS; ++it_DS ){
      if( col > m_DownSampled.size( ).width - 1 ){
        row++;
        col=0;
      } 
      if( ( col % 2 != 0 ) && ( row % 2 != 0) )
      {
        (*it_DS)[ 0 ] = (*it)[ 0 ];
        (*it_DS)[ 1 ] = (*it)[ 1 ];
        (*it_DS)[ 2 ] = (*it)[ 2 ];
        it++;
      }
      else
      {
        (*it_DS)[ 0 ] = 0;
        (*it_DS)[ 1 ] = 0;
        (*it_DS)[ 2 ] = 0;
      }
      col++;
  } //rof

  //Declare GaussianBlur Down Matrix (2048x2048)
  Mat m_BlurredImage;
  m_BlurredImage= Mat::zeros( m_DownSampled.size( ), CV_8UC3);
  //Call convolution function
  GaussianBlur( m_DownSampled, m_BlurredImage, m_GaussianKernel.size( ), 0.85d );
  m_BlurredImage *= 4;

  imwrite( m_Basename + "_Gaussian" + std::to_string(m_Identifier) + ".png", m_BlurredImage );


  //Declare first Down Laplacian Matrix (2048x2048)
  Mat m_DLaplacian1;
  m_DLaplacian1 = Mat::zeros( m_DownSampled.size( ), CV_8UC3);
  //Declare iterators
  MatIterator_< Vec3b > it_DLP, end_DLP, it_DBL, end_DBL;
  it_DS = m_DownSampled.begin< Vec3b >();
  end_DS = m_DownSampled.end< Vec3b >();
  it_DLP = m_DLaplacian1.begin< Vec3b >();
  end_DLP = m_DLaplacian1.end< Vec3b >();
  it_DBL = m_BlurredImage.begin< Vec3b >();
  end_DBL = m_BlurredImage.end< Vec3b >();
  //Down Laplacian Matrix = First Down Sampled Matrix - GaussianBlur Down Matrix 
  for( ;it_DS!= end_DS, it_DBL!= end_DBL, it_DLP!= end_DLP; it_DS++, it_DBL++, it_DLP++){
    (*it_DLP)[0]=(*it_DS)[0]-(*it_DBL)[0];
    (*it_DLP)[1]=(*it_DS)[1]-(*it_DBL)[1];
    (*it_DLP)[2]=(*it_DS)[2]-(*it_DBL)[2];
  } // rof
  imwrite( m_Basename + "_Laplacian" + std::to_string(m_Identifier) + ".png", m_DLaplacian1);
  m_CurrentImage = m_BlurredImage.clone( );

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

   // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );


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
  
  Mat currentImage;
  PyramidDown(image, currentImage, m_GaussianKernel, basename, 4);
  for( int i = 3 ; i > 0 ; i-- )
  {
    PyramidDown( currentImage, currentImage, m_GaussianKernel, basename, i );
  }
  PyramidUp( image, currentImage, m_GaussianKernel, basename, 5);
  for( int i = 6 ; i < 8 ; i++ )
  {
    PyramidUp( currentImage, currentImage, m_GaussianKernel, basename, i );
  }
 
  return( 0 );
}
// eof