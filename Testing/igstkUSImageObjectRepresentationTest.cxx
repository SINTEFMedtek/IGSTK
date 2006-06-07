/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkUSImageObjectRepresentationTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISIS Georgetown University. All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "igstkUSImageObjectRepresentation.h"

#include "itkLogger.h"
#include "itkStdStreamLogOutput.h"

int igstkUSImageObjectRepresentationTest( int , char* [] )
{
  igstk::RealTimeClock::Initialize();

  typedef igstk::USImageObjectRepresentation    RepresentationType;

  RepresentationType::Pointer  representation = RepresentationType::New();

  typedef itk::Logger              LoggerType;
  typedef itk::StdStreamLogOutput  LogOutputType;
  
  // logger object created for logging mouse activities
  LoggerType::Pointer   logger = LoggerType::New();
  LogOutputType::Pointer logOutput = LogOutputType::New();
  logOutput->SetStream( std::cout );
  logger->AddLogOutput( logOutput );
  logger->SetPriorityLevel( itk::Logger::DEBUG );

  representation->SetLogger( logger );

  std::string name = representation->GetNameOfClass();

  std::cout << "Name of class = " << name << std::endl;
    
  representation->Print( std::cout );

  return EXIT_SUCCESS;
}