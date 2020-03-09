#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>

#include <itkImage.h>
#include <itkRGBPixel.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>

#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>




// Image type: 2-dimensional 1-byte rgb
const unsigned int Dim = 2;
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
typedef itk::Image< TRGBPixel, Dim >    TColorImage;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::ImageRegionConstIteratorWithIndex< TColorImage > TIterator;
typedef itk::ImageRegionIteratorWithIndex< TColorImage > TColorIterator;
typedef itk::ImageFileWriter< TColorImage > TWriter;
typedef itk::IdentityTransform<double, 2> TransformType;
typedef itk::ResampleImageFilter< TColorImage, TColorImage > ResampleImageFilterType;
 


// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
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
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( argv[ 1 ] );
  try
  {
    reader->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  TColorImage* img = reader->GetOutput( );
  
  // Create color channel images
  // red channel
  TColorImage::Pointer rImg = TColorImage::New( );
  rImg->SetSpacing( img->GetSpacing( ) );
  rImg->SetOrigin( img->GetOrigin( ) );
  rImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rImg->Allocate( );

  // green channel
  TColorImage::Pointer gImg = TColorImage::New( );
  gImg->SetSpacing( img->GetSpacing( ) );
  gImg->SetOrigin( img->GetOrigin( ) );
  gImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  gImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  gImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  gImg->Allocate( );

  // blue channel
  TColorImage::Pointer bImg = TColorImage::New( );
  bImg->SetSpacing( img->GetSpacing( ) );
  bImg->SetOrigin( img->GetOrigin( ) );
  bImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  bImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  bImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  bImg->Allocate( );

  // composite image (RGB)
  TColorImage::Pointer rgbImg = TColorImage::New( );
  rgbImg->SetSpacing( img->GetSpacing( ) );
  rgbImg->SetOrigin( img->GetOrigin( ) );
  rgbImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rgbImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rgbImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rgbImg->Allocate( );

  // Initialize created images in black
  TRGBPixel black;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );
  rImg->FillBuffer( black );
  gImg->FillBuffer( black );
  bImg->FillBuffer( black );
  rgbImg->FillBuffer( black );

  // Fill color channel images
  TIterator it( img, img->GetLargestPossibleRegion( ) );
  TColorIterator crIt( rImg, rImg->GetLargestPossibleRegion( ) );
  TColorIterator cgIt( gImg, gImg->GetLargestPossibleRegion( ) );
  TColorIterator cbIt( bImg, bImg->GetLargestPossibleRegion( ) );

  it.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  for( ; !it.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !cbIt.IsAtEnd( ); ++it, ++crIt, ++cgIt, ++cbIt )
  {
    TRGBPixel value, pixel;
    pixel = it.Get( );
    value.SetRed( pixel.GetRed( ) );
    value.SetGreen( 0 );
    value.SetBlue( 0 );
    crIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( pixel.GetGreen( ) );
    value.SetBlue( 0 );
    cgIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( 0 );
    value.SetBlue( pixel.GetBlue( ) );
    cbIt.Set( value );

  } // rof

   // Write channels
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );
  writer->SetInput( rImg );
  writer->SetFileName( basename + "_R.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( gImg );
  writer->SetFileName( basename + "_G.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( bImg );
  writer->SetFileName( basename + "_B.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  //Output Sizes for each color
  TColorImage::SizeType inputSize = img->GetLargestPossibleRegion( ).GetSize( );
  std::cout << "Image input size: " << inputSize << std::endl;
  TColorImage::SizeType outputSizeR;
  TColorImage::SizeType outputSizeG;
  TColorImage::SizeType outputSizeB;
  outputSizeR[ 0 ] = inputSize[ 0 ] * 0.75 ;
  outputSizeR[ 1 ] = inputSize[ 1 ] * 0.75 ;
  outputSizeG[ 0 ] = inputSize[ 0 ] * 0.5 ;
  outputSizeG[ 1 ] = inputSize[ 1 ] * 0.5 ;
  outputSizeB[ 0 ] = inputSize[ 0 ] * 0.25 ;
  outputSizeB[ 1 ] = inputSize[ 1 ] * 0.25 ;
  //Output Spacing for each color

  //Red outputSpacing
  TColorImage::SpacingType outputSpacingR;
  outputSpacingR[ 0 ] = 
    img->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeR[ 0 ] ) );
  outputSpacingR[ 1 ] = 
    img->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeR[ 1 ] ) );
  //Green outputSpacing
  TColorImage::SpacingType outputSpacingG;
  outputSpacingG[ 0 ] = 
    img->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeG[ 0 ] ) );
  outputSpacingG[ 1 ] = 
    img->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeG[ 1 ] ) );
  //Blue outputSpacing
  TColorImage::SpacingType outputSpacingB;
  outputSpacingB[ 0 ] = 
    img->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeB[ 0 ] ) );
  outputSpacingB[ 1 ] = 
    img->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeB[ 1 ] ) );


  // Rescale rImg
  ResampleImageFilterType::Pointer resampleFilterR = ResampleImageFilterType::New( );
  resampleFilterR->SetTransform( TransformType::New( ) );
  resampleFilterR->SetInput( rImg );
  resampleFilterR->SetSize( outputSizeR );
  resampleFilterR->SetOutputSpacing( outputSpacingR );
  resampleFilterR->UpdateLargestPossibleRegion( );

  // Rescale gImg
  ResampleImageFilterType::Pointer resampleFilterG = ResampleImageFilterType::New( );
  resampleFilterG->SetTransform( TransformType::New( ) );
  resampleFilterG->SetInput( gImg );
  resampleFilterG->SetSize( outputSizeG );
  resampleFilterG->SetOutputSpacing( outputSpacingG );
  resampleFilterG->UpdateLargestPossibleRegion( );

  // Rescale bImg
  ResampleImageFilterType::Pointer resampleFilterB = ResampleImageFilterType::New( );
  resampleFilterB->SetTransform( TransformType::New( ) );
  resampleFilterB->SetInput( bImg );
  resampleFilterB->SetSize( outputSizeB );
  resampleFilterB->SetOutputSpacing( outputSpacingB );
  resampleFilterB->UpdateLargestPossibleRegion( );
  
   // Write channels rescaled
  

  writer = TWriter::New( );
  writer->SetInput( resampleFilterR->GetOutput() );
  writer->SetFileName( basename + "_sR.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( resampleFilterG->GetOutput() );
  writer->SetFileName( basename + "_sG.png" );
  try
  {
    writer->Update( );
  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( resampleFilterB->GetOutput() );
  writer->SetFileName( basename + "_sB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );
  } // yrt

  //Output Sizes for each color to expand
  TColorImage::SizeType outputSizeREx;
  TColorImage::SizeType outputSizeGEx;
  TColorImage::SizeType outputSizeBEx;
  
  outputSizeREx[ 0 ] = outputSizeR[ 0 ] * 1.333333333333333333333333333333 ;
  outputSizeREx[ 1 ] = outputSizeR[ 1 ] * 1.333333333333333333333333333333 ;
  outputSizeGEx[ 0 ] = outputSizeG[ 0 ] * 2 ;
  outputSizeGEx[ 1 ] = outputSizeG[ 1 ] * 2 ;
  outputSizeBEx[ 0 ] = outputSizeB[ 0 ] * (4) ;
  outputSizeBEx[ 1 ] = outputSizeB[ 1 ] * (4) ;

  //Output Spacing for each color to expand
  //Red outputSpacing
  
  outputSpacingR[ 0 ] = outputSpacingR[ 0 ]/1.333333333333333333333333333333;
  outputSpacingR[ 1 ] = outputSpacingR[ 1 ]/1.333333333333333333333333333333;

  //Green outputSpacing
  
  outputSpacingG[ 0 ] = outputSpacingG[ 0 ]/(2);
  outputSpacingG[ 1 ] = outputSpacingG[ 1 ]/(2);
  //Blue outputSpacing
  
  outputSpacingB[ 0 ] = outputSpacingB[ 0 ]/(4);
  outputSpacingB[ 1 ] = outputSpacingB[ 1 ]/(4);
  
  // Rescale rImg
  ResampleImageFilterType::Pointer resampleFilterREx = ResampleImageFilterType::New( );
  resampleFilterREx->SetTransform( TransformType::New( ) );
  resampleFilterREx->SetInput( resampleFilterR->GetOutput() );
  resampleFilterREx->SetSize( outputSizeREx );
  resampleFilterREx->SetOutputSpacing( outputSpacingR );
  resampleFilterREx->UpdateLargestPossibleRegion( );
  
  // Rescale gImg
  ResampleImageFilterType::Pointer resampleFilterGEx = ResampleImageFilterType::New( );
  resampleFilterGEx->SetTransform( TransformType::New( ) );
  resampleFilterGEx->SetInput( resampleFilterG->GetOutput() );
  resampleFilterGEx->SetSize( outputSizeGEx );
  resampleFilterGEx->SetOutputSpacing( outputSpacingG );
  resampleFilterGEx->UpdateLargestPossibleRegion( );

  // Rescale bImg
  ResampleImageFilterType::Pointer resampleFilterBEx = ResampleImageFilterType::New( );
  resampleFilterBEx->SetTransform( TransformType::New( ) );
  resampleFilterBEx->SetInput( resampleFilterB->GetOutput() );
  resampleFilterBEx->SetSize( outputSizeBEx );
  resampleFilterBEx->SetOutputSpacing( outputSpacingB );
  resampleFilterBEx->UpdateLargestPossibleRegion( );

  writer->SetInput( resampleFilterREx->GetOutput() );
  writer->SetFileName( basename + "_ssR.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  writer->SetInput( resampleFilterGEx->GetOutput() );
  writer->SetFileName( basename + "_ssG.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  writer->SetInput( resampleFilterBEx->GetOutput() );
  writer->SetFileName( basename + "_ssB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  
  // From color channel expanded images, reconstruct the original color image
  
  
  TColorIterator rgbIt( img, img->GetLargestPossibleRegion() );
  TColorIterator cRIt( resampleFilterREx->GetOutput(), resampleFilterREx->GetOutput()->GetLargestPossibleRegion() );
  TColorIterator cGIt( resampleFilterGEx->GetOutput(), resampleFilterGEx->GetOutput()->GetLargestPossibleRegion() );
  TColorIterator cBIt( resampleFilterBEx->GetOutput(), resampleFilterBEx->GetOutput()->GetLargestPossibleRegion() );
  rgbIt.GoToBegin( );
  cRIt.GoToBegin( );
  cGIt.GoToBegin( );
  cBIt.GoToBegin( );
  for( ; !rgbIt.IsAtEnd( ) && !cRIt.IsAtEnd( ) && !cGIt.IsAtEnd( ) && !cBIt.IsAtEnd( ); ++rgbIt, ++cRIt, ++cGIt, ++cBIt )
  {
    TRGBPixel value, pixel;
    value = cRIt.Get( );
    pixel.SetRed( value.GetRed( ) );

    value = cGIt.Get( );
    pixel.SetGreen( value.GetGreen( ) );

    value = cBIt.Get( );
    pixel.SetBlue( value.GetBlue( ) );

    rgbIt.Set( pixel );

  } // rof
  
 
  //Write de rescaled RGB Image
  writer->SetInput( img );
  writer->SetFileName( basename + "_rRGB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );
  } // yrt
  
  //Find diferences
  TColorImage* diferences = reader->GetOutput( );
  //Compare Original and Rescaled Images
  TColorIterator difIt( diferences, diferences->GetLargestPossibleRegion( ) );
  difIt.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  cRIt.GoToBegin( );
  cGIt.GoToBegin( );
  cBIt.GoToBegin( );
  for( ; !cbIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cBIt.IsAtEnd( ) && !cGIt.IsAtEnd( ) && !cRIt.IsAtEnd( ) && !difIt.IsAtEnd(); ++cbIt, ++cgIt, ++crIt, ++cBIt, ++cGIt, ++cRIt, ++difIt)
  {
    TRGBPixel value, pixel;
    value = crIt.Get()-cRIt.Get();
    pixel.SetRed(value.GetRed());

    value = cgIt.Get()-cGIt.Get();
    pixel.SetGreen(value.GetGreen());

    value = cbIt.Get()-cBIt.Get();
    pixel.SetBlue(value.GetBlue());

    difIt.Set(pixel);

  } // rof
  
  //Write diferences
  writer->SetInput( diferences );
  writer->SetFileName( basename + "_diff.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  return( 0 );
}

// eof - 03_image_process_itk.cxx
