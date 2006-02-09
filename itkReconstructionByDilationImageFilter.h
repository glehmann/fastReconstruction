#ifndef __itkReconstructionByDilationImageFilter_h
#define __itkReconstructionByDilationImageFilter_h

#include "itkReconstructionImageFilter.h"

#include "itkNumericTraits.h"

namespace itk {
/** \class ReconstructionByDilationImageFilter
 * \brief A grayscale geodesic dilation. No incremental option
 * available. Should be much faster than the current ITK version.
 * \author Richard Beare. Department of Medicine, Monash University,
 * Melbourne, Australia.
 * \ingroup MathematicalMorphologyImageFilters
*/

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReconstructionByDilationImageFilter :
    public
    ReconstructionImageFilter<TInputImage, TOutputImage,
					   std::greater<typename TOutputImage::PixelType>,
					   std::less<typename TOutputImage::PixelType>  >{
public:
  typedef ReconstructionByDilationImageFilter Self;
  typedef ReconstructionImageFilter<TInputImage, TOutputImage,
                                     std::greater<typename TOutputImage::PixelType>,
                                     std::less<typename TOutputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  ReconstructionByDilationImageFilter()
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::NonpositiveMin());
  }
  virtual ~ReconstructionByDilationImageFilter() {}

private:
  ReconstructionByDilationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end ReconstructionByDilationImageFilter



}

#endif
