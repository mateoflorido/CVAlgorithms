/*#include <cmath>
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

#include "itkGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkScalarToRGBColormapImageFilter.h>
#include "itkGradientMagnitudeImageFilter.h"
#include <itkRGBToLuminanceImageFilter.h>
#include <itkMorphologicalWatershedImageFilter.h>
#include <itkConfidenceConnectedImageFilter.h>
#include <itkImage.h>
#include <itksys/SystemTools.hxx>



// Image type: 2-dimensional 1-byte rgb
const unsigned int Dim = 2;
constexpr unsigned int Dimension = 2;
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
 
//Aliases
using ImageType = itk::Image<unsigned char, Dimension>;
using ImageType = itk::Image<unsigned char, 2>;
using FloatImageType = itk::Image<float, Dimension>;
using RGBPixelType = itk::RGBPixel<unsigned char>;
using RGBImageType = itk::Image<RGBPixelType, Dimension>;
using LabeledImageType = itk::Image<itk::IdentifierType, Dimension>;
using FileReaderType = itk::ImageFileReader<ImageType>;
using GradientMagnitudeImageFilterType = itk::GradientMagnitudeImageFilter<ImageType, FloatImageType>;
using WatershedFilterType = itk::WatershedImageFilter<FloatImageType>;
using RGBFilterType = itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType>;
using FileWriterType = itk::ImageFileWriter<RGBImageType>;
using MorphologicalWatershedFilterType = itk::MorphologicalWatershedImageFilter<FloatImageType, LabeledImageType>;
using ConfidenceConnectedFilterType = itk::ConfidenceConnectedImageFilter<ImageType, LabeledImageType>;

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
  FileReaderType::Pointer reader = FileReaderType::New();
  std::stringstream ss( argv[ 1 ] );
  std::string basename;

  reader->SetFileName( argv[ 1 ] );
  try
  {
    reader->Update( );
    getline( ss, basename, '.' );
  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  // ----- Greyscale Process -----


  //------ Watersheds Process -------
  GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter =GradientMagnitudeImageFilterType::New();
  gradientMagnitudeImageFilter->SetInput(reader->GetOutput());

  WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();
  float threshold = 0.005;
  float level = .5;
  watershedFilter->SetLevel(level);
  watershedFilter->SetThreshold(threshold);

  watershedFilter->SetInput(gradientMagnitudeImageFilter->GetOutput());
  watershedFilter->Update();

  RGBFilterType::Pointer colormapFilter = RGBFilterType::New( );
  colormapFilter->SetColormap(RGBFilterType::Jet);
  colormapFilter->SetInput(watershedFilter->GetOutput( ) );
  colormapFilter->Update( );


  // ----- Morphological Watershed -----
  
  MorphologicalWatershedFilterType::Pointer morphWatershed = MorphologicalWatershedFilterType::New( );
  morphWatershed->SetInput(gradientMagnitudeImageFilter->GetOutput( ));
  morphWatershed->SetLevel(1.8);
  morphWatershed->Update();
  RGBFilterType::Pointer colormapMorph = RGBFilterType::New();
  colormapMorph->SetInput(morphWatershed->GetOutput());
  colormapMorph->SetColormap(RGBFilterType::Jet);
  colormapMorph->Update();

  // ----- Connectivity Region Growing -----
  ImageType::IndexType seed;
  seed[ 0 ] = 430;
  seed[ 1 ] = 666;

  ConfidenceConnectedFilterType::Pointer confidenceFilter = ConfidenceConnectedFilterType::New();
  confidenceFilter->SetInitialNeighborhoodRadius(3);
  confidenceFilter->SetMultiplier(3);
  confidenceFilter->SetNumberOfIterations(50);
  confidenceFilter->SetReplaceValue(255);
  confidenceFilter->SetSeed(seed);
  reader->Update();
  confidenceFilter->SetInput(reader->GetOutput());
  confidenceFilter->Update();


  RGBFilterType::Pointer colormapConfidence = RGBFilterType::New();
  colormapConfidence->SetInput(confidenceFilter->GetOutput());
  colormapConfidence->SetColormap(RGBFilterType::Hot);
  colormapConfidence->Update();

  FileWriterType::Pointer writer = FileWriterType::New( );
  //Write WaterSheds Result
  writer->SetInput( colormapFilter->GetOutput( ) );
  writer->SetFileName( basename + "_Watersheds.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error Writing: " << err << std::endl;
    return( EXIT_FAILURE );

  } // yrt
  //Write WaterSheds Result
  writer->SetInput( colormapMorph->GetOutput( ) );
  writer->SetFileName( basename + "_MorphWatersheds.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error Writing: " << err << std::endl;
    return( EXIT_FAILURE );

  } // yrt
  //Write WaterSheds Result
  writer->SetInput( colormapConfidence->GetOutput() );
  writer->SetFileName( basename + "_RegionGrowing.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error Writing: " << err << std::endl;
    return( EXIT_FAILURE );

  } // yrt
  return( EXIT_SUCCESS );
}

// eof - segmentation.cxx
