/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkAuroraTrackerTest2.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
//  Warning about: identifier was truncated to '255' characters 
//  in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <fstream>
#include <set>

#include "itkCommand.h"
#include "igstkLogger.h"
#include "itkStdStreamLogOutput.h"
#include "itkVector.h"
#include "itkVersor.h"

#include "igstkSystemInformation.h"
#include "igstkSerialCommunication.h"
#include "igstkAuroraTracker.h"
#include "igstkAuroraTrackerTool.h"
#include "igstkTransform.h"

class AuroraTrackerTestCommand : public itk::Command 
{
public:
  typedef  AuroraTrackerTestCommand     Self;
  typedef  itk::Command                 Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  itkNewMacro( Self );
protected:
  AuroraTrackerTestCommand() {};

public:
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event)
    {
    // don't print "CompletedEvent", only print interesting events
    if (!igstk::CompletedEvent().CheckEvent(&event) &&
        !itk::DeleteEvent().CheckEvent(&event) )
      {
      std::cout << event.GetEventName() << std::endl;
      }
    }
};

//This program tests using 5DOF tracker tools
int igstkAuroraTrackerTest2( int argc, char * argv[] )
{

  igstk::RealTimeClock::Initialize();

  typedef igstk::Object::LoggerType     LoggerType;
  typedef itk::StdStreamLogOutput       LogOutputType;

  if( argc < 3 )
    {
    std::cerr << " Usage: " << argv[0] << "\t" 
                            << "Logger_Output_filename " <<"\t"
                            << "PortA_number" << "\t"
                            << "PortB_number" << "\t"
                            << "Order" << "\t"
                            << "UseSpliter?" << "\t"
                            << std::endl;
    return EXIT_FAILURE;
    }


  igstk::AuroraTrackerTool::Pointer tool = igstk::AuroraTrackerTool::New();

  igstk::SerialCommunication::Pointer 
                     serialComm = igstk::SerialCommunication::New();

  AuroraTrackerTestCommand::Pointer 
                                my_command = AuroraTrackerTestCommand::New();

  std::string filename = argv[1];
  std::cout << "Logger output saved here:\n";
  std::cout << filename << "\n"; 

  std::ofstream loggerFile;
  loggerFile.open( filename.c_str() );
  LoggerType::Pointer   logger = LoggerType::New();
  LogOutputType::Pointer logOutput = LogOutputType::New();  
  logOutput->SetStream( loggerFile );
  logger->AddLogOutput( logOutput );
  logger->SetPriorityLevel( LoggerType::DEBUG);

  serialComm->AddObserver( itk::AnyEvent(), my_command);

  serialComm->SetLogger( logger );

  serialComm->SetPortNumber( IGSTK_TEST_AURORA_PORT_NUMBER );
  serialComm->SetParity( igstk::SerialCommunication::NoParity );
  serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate115200 );
  serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
  serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
  serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );

  serialComm->SetCaptureFileName( "RecordedStreamByAuroraTrackerTest.txt" );
  serialComm->SetCapture( true );

  serialComm->OpenCommunication();

  igstk::AuroraTracker::Pointer  tracker;

  tracker = igstk::AuroraTracker::New();

  tracker->AddObserver( itk::AnyEvent(), my_command);

  tracker->SetLogger( logger );

  std::cout << "SetCommunication()" << std::endl;
  tracker->SetCommunication( serialComm );

  std::cout << "RequestOpen()" << std::endl;
  tracker->RequestOpen();

  typedef igstk::AuroraTrackerTool          TrackerToolType;
  typedef TrackerToolType::TransformType    TransformType;

  unsigned int portANumber = atoi(argv[2]);
  unsigned int portBNumber = atoi(argv[3]);
  unsigned int order = atoi(argv[4]);
  unsigned int spliter = atoi(argv[5]);

  // instantiate tracker tool with 5DOF 
  TrackerToolType::Pointer trackerTool = TrackerToolType::New();
  trackerTool->SetLogger( logger );
  trackerTool->RequestSelect5DOFTrackerTool();
  //Set the port number 
  trackerTool->RequestSetPortNumber( portANumber );
  //Set channel number to zero
  trackerTool->RequestSetChannelNumber( 0 );
 

  // instantiate a second tracker tool with 5DOF 
  TrackerToolType::Pointer trackerTool2 = TrackerToolType::New();
  trackerTool2->SetLogger( logger );
  trackerTool2->RequestSelect5DOFTrackerTool();
  //Set the port number 
  trackerTool2->RequestSetPortNumber( portANumber );
  //Set channel number to one 
  trackerTool2->RequestSetChannelNumber( 1 );


  // instantiate a third tracker tool with 5DOF 
  TrackerToolType::Pointer trackerTool3 = TrackerToolType::New();
  trackerTool3->SetLogger( logger );
  trackerTool3->RequestSelect5DOFTrackerTool();
  //Set the port number 
  trackerTool3->RequestSetPortNumber( portBNumber );
  //Set channel number to one 
  trackerTool3->RequestSetChannelNumber( 0 );


  if( order == 0)
    { 
    // These two are on the same port using spliter
    std::cout << "Attaching tool 1\n";
    trackerTool->RequestConfigure();
    trackerTool->RequestAttachToTracker( tracker );
    if( spliter )
      {
      std::cout << "Attaching tool 2\n";
      trackerTool2->RequestConfigure();
      trackerTool2->RequestAttachToTracker( tracker );
      }

    // This is a single 5DOF tool plugged directly into the port
    std::cout << "Attaching tool 3\n";
    trackerTool3->RequestConfigure();
    trackerTool3->RequestAttachToTracker( tracker );
    }
  else
    {
    // This is a single 5DOF tool plugged directly into the port
    std::cout << "Attaching tool 3\n";
    trackerTool3->RequestConfigure();
    trackerTool3->RequestAttachToTracker( tracker );

    // These two are on the same port using spliter
    std::cout << "Attaching tool 1\n";
    trackerTool->RequestConfigure();
    trackerTool->RequestAttachToTracker( tracker );
    if( spliter )
      {
      std::cout << "Attaching tool 2\n";
      trackerTool2->RequestConfigure();
      trackerTool2->RequestAttachToTracker( tracker );
      }
    }

  //start tracking 
  tracker->RequestStartTracking();

  typedef igstk::Transform            TransformType;
  typedef ::itk::Vector<double, 3>    VectorType;
  typedef ::itk::Versor<double>       VersorType;


  for(unsigned int i=0; i<100; i++)
    {
    tracker->RequestUpdateStatus();

    TransformType             transform;
    VectorType                position;

    transform = trackerTool->GetCalibratedTransform();
    position = transform.GetTranslation();
    std::string toolString = trackerTool->GetTrackerToolIdentifier();
    std::cout << "Trackertool:" << toolString
              << "  Position = (" << position[0]
              << "," << position[1] << "," << position[2]
              << ")" << std::endl;

    if( spliter )
      {
      transform = trackerTool2->GetCalibratedTransform();
      position = transform.GetTranslation();
      toolString = trackerTool2->GetTrackerToolIdentifier();
      std::cout << "Trackertool2:" << toolString
                << "  Position = (" << position[0]
                << "," << position[1] << "," << position[2]
                << ")" << std::endl;
      }

    transform = trackerTool3->GetCalibratedTransform();
    position = transform.GetTranslation();
    toolString = trackerTool3->GetTrackerToolIdentifier();
    std::cout << "Trackertool3:" << toolString
                << "  Position = (" << position[0]
              << "," << position[1] << "," << position[2]
              << ")" << std::endl;
    }
  
  std::cout << "RequestStopTracking()" << std::endl;
  tracker->RequestStopTracking();

  std::cout << "RequestClose()" << std::endl;
  tracker->RequestClose();

  std::cout << "CloseCommunication()" << std::endl;
  serialComm->CloseCommunication();

  std::cout << "[PASSED]" << std::endl;


  return EXIT_SUCCESS;
}
