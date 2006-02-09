#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkHMaximaImageFilterRobinson.h"
#include "itkHMaximaImageFilter.h"
#include "itkSimpleFilterWatcher.h"

int main(int, char * argv[])
{

  const int dim = 2;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  
  int height = atoi(argv[4]);
  int F = atoi(argv[5]);
  typedef itk::HMaximaImageFilter< IType, IType > ReconType;
  ReconType::Pointer recon = ReconType::New();
  itk::SimpleFilterWatcher watcher(recon, "recon");
  recon->SetInput( reader->GetOutput() );
  recon->SetHeight(height);
 
  typedef itk::HMaximaImageFilterRobinson< IType, IType > OrigReconType;
  OrigReconType::Pointer origrecon = OrigReconType::New();
  origrecon->SetInput( reader->GetOutput() );
  origrecon->SetHeight(height);

  origrecon->SetFullyConnected(F);
  recon->SetFullyConnected(F);
    
  origrecon->Update();

  recon->Update();

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );

  writer->SetInput(recon->GetOutput());
  writer->Update();

  writer->SetFileName(argv[3]);
  writer->SetInput(origrecon->GetOutput());
  writer->Update();
  

  return 0;
}

