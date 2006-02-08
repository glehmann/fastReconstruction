#ifndef __itkMorphologicalReconstructionImageFilter_txx
#define __itkMorphologicalReconstructionImageFilter_txx

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkMorphologicalReconstructionImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConnectedComponentAlgorithm.h"
#include <queue>

namespace itk {

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::MorphologicalReconstructionImageFilter()
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  MarkerImagePointer  markerPtr =
    const_cast< MarkerImageType * >( this->GetInput(0) );

  MaskImagePointer  maskPtr =
    const_cast< MaskImageType * >( this->GetInput(1) );

  if ( !markerPtr || !maskPtr )
    {
    return;
    }
  markerPtr->SetRequestedRegion(markerPtr->GetLargestPossibleRegion());
  maskPtr->SetRequestedRegion(maskPtr->GetLargestPossibleRegion());
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::SetMarkerImage(const MarkerImageType* markerImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<MarkerImageType *>( markerImage ));
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
const typename MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>::MarkerImageType *
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>::GetMarkerImage()
{
  return this->GetInput(0);
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>::SetMaskImage(const MaskImageType* maskImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<MaskImageType *>( maskImage ));
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
const typename MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>::MaskImageType *
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>::GetMaskImage()
{
  return this->GetInput(1);
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  // there are 2 passes that use all pixels and a 3rd that uses some
  // subset of the pixels. We'll just pretend that the third pass
  // takes the same as each of the others. Is it OK to update more
  // often than pixels?
  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*3);

  typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef ImageRegionIterator<OutputImageType> OutputIteratorType;
  
  MarkerImageConstPointer markerImage = this->GetMarkerImage();
  MaskImageConstPointer   maskImage = this->GetMaskImage();
  OutputImagePointer      output = this->GetOutput();
  
  TFunction1 compareA;
  TFunction2 compareB;

  InputIteratorType inIt( markerImage,
			  output->GetRequestedRegion() );
  OutputIteratorType outIt( output,
			    output->GetRequestedRegion() );
  inIt.GoToBegin();
  outIt.GoToBegin();

  // declare our queue type
  typedef typename std::queue<OutputImageIndexType> FifoType;
  FifoType IndexFifo;

  // copy marker to output - isn't there a better way?
  while ( !outIt.IsAtEnd() )
    {
    MarkerImagePixelType currentValue = inIt.Get();
    outIt.Set( static_cast<OutputImagePixelType>( currentValue ) );
    ++inIt;
    ++outIt;
    }
  ISizeType kernelRadius;
  kernelRadius.Fill(1);
  NOutputIterator outNIt(kernelRadius,
			 output,
			 output->GetRequestedRegion() );
  setConnectivityPrev( &outNIt, m_FullyConnected );
  
  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);
  outNIt.OverrideBoundaryCondition(&oBC);
  
  InputIteratorType mskIt( maskImage,
			   output->GetRequestedRegion() );
  
  mskIt.GoToBegin();
  // scan in forward raster order
  for (outNIt.GoToBegin(),mskIt.GoToBegin();!outNIt.IsAtEnd(); ++outNIt,++mskIt)
    {
    OutputImagePixelType V = outNIt.GetCenterPixel();
    // visit the previous neighbours
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compareA(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskIt.Get());
    if (compareA(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      }
    progress.CompletedPixel();
    }
  // now for the reverse raster order pass
  // reset the neighborhood
  setConnectivityLater( &outNIt, m_FullyConnected );
  outNIt.OverrideBoundaryCondition(&oBC);
  outNIt.GoToEnd();
  //mskIt.GoToEnd();
  
  ConstantBoundaryCondition<InputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);
  CNInputIterator mskNIt(kernelRadius,
			 maskImage,
			 output->GetRequestedRegion() );
  
  setConnectivityLater( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  
  typename NOutputIterator::IndexListType oIndexList, mIndexList;
  typename NOutputIterator::IndexListType::const_iterator oLIt, mLIt;
  
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  
  mskNIt.GoToEnd();
  while (!outNIt.IsAtBegin())
    {
    --outNIt;
    --mskNIt;
    OutputImagePixelType V = outNIt.GetCenterPixel();
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compareA(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    //std::cout << (int)V << " " << mskNIt.GetIndex() <<std::endl;
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskNIt.GetCenterPixel());
    if (compareA(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      V = iV;
      }

    // now put indexes in the fifo
    //typename CNInputIterator::ConstIterator mIt;
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); oLIt != oIndexList.end();++oLIt, ++mLIt)
      {
      
      //std::cout << " " << outNIt.GetIndex(*oLIt);
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      if (compareB(VN, V) && compareB(VN, iN)) 
	{
	IndexFifo.push(outNIt.GetIndex());
	break;
	}
      }
    progress.CompletedPixel();
    }

  
  // Now we want to check the full neighborhood
  setConnectivity( &outNIt, m_FullyConnected );
  setConnectivity( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  outNIt.OverrideBoundaryCondition(&oBC);
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  // now process the fifo - this fill the parts that weren't dealt
  // with by the raster and anti-raster passes
  //typename NOutputIterator::Iterator sIt;
  typename CNInputIterator::ConstIterator mIt;
  
  while (!IndexFifo.empty())
    {
    OutputImageIndexType I = IndexFifo.front();
    IndexFifo.pop();
    // reposition the iterators
    outNIt += I - outNIt.GetIndex();
    mskNIt += I - mskNIt.GetIndex();
    OutputImagePixelType V = outNIt.GetCenterPixel();
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); 
	 oLIt != oIndexList.end();
	 ++oLIt, ++mLIt)
      {
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      // candidate for dilation via flooding
      if (compareB(VN, V) && (iN != VN))
	{
	if (compareB(V, iN)) 
	  {
	  // not clamped by the mask, propogate the center value
	  outNIt.SetPixel(*oLIt, V);
	  }
	else
	  {
	  // apply the clamping
	  outNIt.SetPixel(*oLIt, iN);
	  }
	 IndexFifo.push(outNIt.GetIndex(*oLIt));
	}
      }     
    progress.CompletedPixel();
    }
  
}
template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
MorphologicalReconstructionImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "boundary value: " << m_MarkerValue << std::endl;
}
}
#endif
