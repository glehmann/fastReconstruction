#ifndef __itkMorphologicalReconstructionDilationImageFilter_h
#define __itkMorphologicalReconstructionDilationImageFilter_h

#include "itkMorphologicalReconstructionImageFilter.h"

#include "itkNumericTraits.h"

namespace itk {
/** \class MorphologicalReconstructionDilationImageFilter
 * \brief A grayscale geodesic dilation. No incremental option
 * available. Should be much faster than the current ITK version.
 * \author Richard Beare. Department of Medicine, Monash University,
 * Melbourne, Australia.
 * \ingroup MathematicalMorphologyImageFilters
*/

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MorphologicalReconstructionDilationImageFilter :
    public
    MorphologicalReconstructionImageFilter<TInputImage, TOutputImage,
					   std::greater<typename TOutputImage::PixelType>,
					   std::less<typename TOutputImage::PixelType>  >{
public:
  typedef MorphologicalReconstructionDilationImageFilter Self;
  typedef MorphologicalReconstructionImageFilter<TInputImage, TOutputImage,
                                     std::greater<typename TOutputImage::PixelType>,
                                     std::less<typename TOutputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  MorphologicalReconstructionDilationImageFilter()
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::NonpositiveMin());
  }
  virtual ~MorphologicalReconstructionDilationImageFilter() {}

private:
  MorphologicalReconstructionDilationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end MorphologicalReconstructionDilationImageFilter



}

#endif
