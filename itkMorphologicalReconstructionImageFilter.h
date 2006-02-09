#ifndef __itkMorphologicalReconstructionImageFilter_h
#define __itkMorphologicalReconstructionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include <queue>

//#define BASIC
//#define FACES
#define COPY

#ifdef FACES
#include "itkNeighborhoodAlgorithm.h"
#endif

#ifdef COPY
#include "itkNeighborhoodAlgorithm.h"
#endif

namespace itk {

/** \class MorphologicalReconstructionImageFilter 
 * \brief Performs a grayscale geodesic reconstruction -- for
 * performance comparison with GrayscaleGeodesicDilateImageFilter. 
 *
 * This filter uses Luc Vincent's algorithm, which employs raster and
 * antiraster propogation steps followed by a FIFO based propogation
 * step. "Morphological grayscale reconstruction in image analysis -
 * applications and efficient algorithms" -- IEEE Transactions on
 * Image processing, Vol 2, No 2, pp 176-201, April 1993
 *
 * \author Richard Beare. Department of Medicine, Monash University,
 * Melbourne, Australia.
 *
 * \sa itkMorphologicalReonstructionErosionImageFilter itkMorphologicalReonstructionDilationImageFilter
 * \ingroup MathematicalMorphologyImageFilters
 */

template<class TInputImage, class TOutputImage, 
	 class TFunction1, class TFunction2>
class ITK_EXPORT MorphologicalReconstructionImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MorphologicalReconstructionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TInputImage MarkerImageType;
  typedef TInputImage MaskImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::SizeType        ISizeType;
  typedef typename MarkerImageType::Pointer        MarkerImagePointer;
  typedef typename MarkerImageType::ConstPointer   MarkerImageConstPointer;
  typedef typename MarkerImageType::RegionType     MarkerImageRegionType;
  typedef typename MarkerImageType::PixelType      MarkerImagePixelType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::IndexType       InputImageIndexType;
  typedef typename MaskImageType::Pointer          MaskImagePointer;
  typedef typename MaskImageType::ConstPointer     MaskImageConstPointer;
  typedef typename MaskImageType::RegionType       MaskImageRegionType;
  typedef typename MaskImageType::PixelType        MaskImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      OutputImageIndexType;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  
  /** ImageDimension constants */
  /** ImageDimension constants */
  itkStaticConstMacro(MarkerImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(MaskImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(MorphologicalReconstructionImageFilter, 
               ImageToImageFilter);

  /** Set/Get the marker image. Traditionally, the marker image must
   * be pixelwise less than or equal to the mask image (for dilation),
   * however this filter implicitly applies a mask to force the
   * constraint to hold. The marker image the
   * image that is dilated by this filter. */
  void SetMarkerImage(const MarkerImageType *);
  const MarkerImageType* GetMarkerImage();

  /** Set/Get the mask image. The mask image is used to "mask" the
   * dilated marker image. The mask operation is a pixelwise
   * minimum. */
  void SetMaskImage(const MaskImageType *);
  const MaskImageType* GetMaskImage();


  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);
  
  /**
   * Set/Get the value of the border - used in boundary condition.
   */
  itkSetMacro(MarkerValue, typename TInputImage::PixelType);
  itkGetConstReferenceMacro(MarkerValue, typename TInputImage::PixelType);

 
protected:
  MorphologicalReconstructionImageFilter();
  ~MorphologicalReconstructionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** ValuedRegionalExtremaImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** ValuedRegionalExtremaImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  void GenerateData();
  

private:
  MorphologicalReconstructionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  typename TInputImage::PixelType m_MarkerValue;
  bool                m_FullyConnected;

#ifdef FACES
  TFunction1 compareA;
  TFunction2 compareB;


  // declare our queue type
  typedef typename std::queue<OutputImageIndexType> FifoType;

  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<OutputImageType> FaceCalculatorType;

  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceCalculatorType::FaceListType::iterator FaceListTypeIt;

  void processRegion(ProgressReporter &progress,
		     const OutputImageRegionType thisRegion,
		     const ISizeType kernelRadius,
		     MarkerImageConstPointer markerImage,
		     MaskImageConstPointer   maskImage,
		     OutputImagePointer &output,
		     FifoType &IndexFifo);

  void buildFifo(ProgressReporter &progress,
		 const OutputImageRegionType thisRegion,
		 OutputImagePointer &output,
		 FifoType &IndexFifo);

  void processFifo(ProgressReporter &progress,
		   const OutputImageRegionType thisRegion,
		   const ISizeType kernelRadius,
		   MarkerImageConstPointer markerImage,
		   MaskImageConstPointer   maskImage,
		   OutputImagePointer &output,
		   FifoType &IndexFifo);

  void fillFaces(FaceListType faceList,
		 OutputImagePointer &output);
  
  void copyFaces(FaceListType faceList,
		 MarkerImageConstPointer markerImage,
		 OutputImagePointer &output);
		 
#endif

#ifdef COPY
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<OutputImageType> FaceCalculatorType;

  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceCalculatorType::FaceListType::iterator FaceListTypeIt;
#endif
  typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef ImageRegionIterator<OutputImageType> OutputIteratorType;

  typedef typename OutputImageType::IndexType OutIndexType;
  typedef typename InputImageType::IndexType InIndexType;
  typedef ConstShapedNeighborhoodIterator<InputImageType> CNInputIterator;
  typedef ShapedNeighborhoodIterator<OutputImageType> NOutputIterator;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMorphologicalReconstructionImageFilter.txx"
#endif


#endif
