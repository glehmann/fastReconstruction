#ifndef __itkHMaximaImageFilterRobinson_txx
#define __itkHMaximaImageFilterRobinson_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkHMaximaImageFilterRobinson.h"
#include "itkReconstructionByDilationImageFilterRobinson.h"
#include "itkShiftScaleImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
HMaximaImageFilterRobinson<TInputImage, TOutputImage>
::HMaximaImageFilterRobinson()
{
  m_Height =  2;
  m_NumberOfIterationsUsed = 1;
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage>
void 
HMaximaImageFilterRobinson<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
HMaximaImageFilterRobinson<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
HMaximaImageFilterRobinson<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // construct a marker image to manipulate using reconstruction by
  // erosion. the marker image is the input image minus the height
  // parameter.
  typedef ShiftScaleImageFilter<TInputImage, TInputImage>
    ShiftFilterType;
  typename ShiftFilterType::Pointer shift = ShiftFilterType::New();
  shift->SetInput( this->GetInput() );
  shift->SetShift( -1.0 * static_cast<typename ShiftFilterType::RealType>(m_Height) );

  // Delegate to a geodesic erosion filter.
  //
  //
  typename ReconstructionByDilationImageFilterRobinson<TInputImage, TInputImage>::Pointer
    dilate
    = ReconstructionByDilationImageFilterRobinson<TInputImage, TInputImage>::New();

  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(dilate,1.0f);

  // set up the dilate filter
  //dilate->RunOneIterationOff();             // run to convergence
  dilate->SetMarkerImage( shift->GetOutput() );
  dilate->SetMaskImage( this->GetInput() );
  dilate->SetFullyConnected( m_FullyConnected );

  // graft our output to the dilate filter to force the proper regions
  // to be generated
  dilate->GraftOutput( this->GetOutput() );

  // reconstruction by erosion
  dilate->Update();

  // graft the output of the dilate filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( dilate->GetOutput() );
}


template<class TInputImage, class TOutputImage>
void
HMaximaImageFilterRobinson<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Depth of local maxima (contrast): "
     << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_Height)
     << std::endl;
  os << indent << "Number of iterations used to produce current output: "
     << m_NumberOfIterationsUsed << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
