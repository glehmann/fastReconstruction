#include "itkImageFileReader.h"

#include "itkHMinimaImageFilter2.h"
#include "itkHMinimaImageFilter.h"
#include "itkTimeProbe.h"
#include <vector>

int main(int, char * argv[])
{

  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  
  
  typedef itk::HMinimaImageFilter2< IType, IType > ReconType;
  ReconType::Pointer recon = ReconType::New();
  recon->SetInput( reader->GetOutput() );
  recon->SetHeight(1);
 
  typedef itk::HMinimaImageFilter< IType, IType > OrigReconType;
  OrigReconType::Pointer origrecon = OrigReconType::New();
  origrecon->SetInput( reader->GetOutput() );
  origrecon->SetHeight(1);
 
  std::cout << "#F" << "\t"
            << "original" << "\t"
            << "vincent" << "\t"
            << std::endl;

  itk::TimeProbe origtime;
  itk::TimeProbe vincenttime;

  for(int F=0; F<=1; F++ )
    {
    origrecon->SetFullyConnected(F);
    recon->SetFullyConnected(F);
    
    for (int i=0;i<10;i++)
      {
#if 0
      origtime.Start();
      origrecon->Update();
      origtime.Stop();
      origrecon->Modified();
#endif
      vincenttime.Start();
      recon->Update();
      vincenttime.Stop();
      recon->Modified();
      
      }

    std::cout << F << "\t"
              << origtime.GetMeanTime() << "\t"
              << vincenttime.GetMeanTime() << "\t"
              <<std::endl;
    }

  return 0;
}

