/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    HelloWorld.cxx
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
//Warning about: identifier was truncated to '255' characters 
//in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4284 )
#endif

// BeginLatex
//
// To add a graphical user interface to the application, we use FLTK.
// FLTK is a a light weight cross-platform GUI toolkit. 
// FLTK stores a description of an interface in files with extension .fl. 
// The FLTK tool \emph{fluid} takes this file and uses it for generating C++ 
// code in two files: one header file with extension .h, and an implementation
// file with extension .cxx. In order to use the GUI from the main program of 
// our application, we must include the header file generated by \emph{fluid}. 
// This is done as follows:.
// 
// EndLatex

// BeginCodeSnippet
#include "HelloWorldGUI.h"
// EndCodeSnippet


// BeginLatex
//
// The geometrical description of the Cylinder and the Sphere in the scene are
// managed by SpatialObjects. For this purpose we need two classes,
// \doxygen{EllipsoidObject} and \doxygen{CylinderObject}. Their two header
// files are:
// 
// EndLatex

// BeginCodeSnippet
#include "igstkEllipsoidObject.h"
#include "igstkCylinderObject.h"
// EndCodeSnippet

// BeginLatex
//
// The visual representation of SpatialObjects in the visualization window is
// created using SpatialObject Representation classes. 
// Every SpatialObject has one or several representation objects 
// associated with it. Here we include the header files
// of the \doxygen{EllipsoidObjectRepresentation} and
// \doxygen{CylinderObjectRepresentation}:
// 
// EndLatex

// BeginCodeSnippet
#include "igstkEllipsoidObjectRepresentation.h"
#include "igstkCylinderObjectRepresentation.h"
// EndCodeSnippet


// BeginLatex
//
// As stated above, the tracker in this minimal application is
// represented by a \doxygen{MouseTracker}. This class provides the same
// interface as a real tracking device but with the convenience of running
// based on the movement of the mouse in the screen. The header file for this
// class is as follows:
// 
// EndLatex

// BeginCodeSnippet
#include "igstkMouseTracker.h"
// EndCodeSnippet


// BeginLatex
//
// Since image-guided surgery applications are used in a safety-critical
// environment, it is important to be able to trace the behavior of the
// application during the intervention. For this purpose, IGSTK uses a
// \doxygen{Logger} class and some helpers. The logger is a class that receives
// messages from IGSTK classes and forwards those messages to LoggerOutput
// classes. Typical logger output classes are the standard output, a file, and
// a popup window. The Logger classes and their helpers are taken from the
// Insight Toolkit (ITK): 
// 
// EndLatex

// BeginCodeSnippet
#include "itkLogger.h"
#include "itkStdStreamLogOutput.h"
// EndCodeSnippet


// BeginLatex
//
// We are now ready to write the code of the actual application. Of course, we
// start with the classical \code{main()} function:
// 
// EndLatex

// BeginCodeSnippet
int main(int , char** )
{ 
  // EndCodeSnippet

  // BeginLatex 
  // The first IGSTK command to be invoked in an application is the one that
  // initializes the parameters of the clock. Timing is critical for all
  // operations performed in an IGS application. Timing signals make it possible
  // to synchronize the operation of different components and to ensure that the
  // scene rendered on the screen actually displays a consistent 
  // state of the environment on the operating room. The command for the timer
  // initialization is as follows:
  // EndLatex
  // 
  // BeginCodeSnippet
  igstk::RealTimeClock::Initialize();
  // EndCodeSnippet

  // BeginLatex
  // First, we instantiate the GUI application:
  // EndLatex
  // 
  // BeginCodeSnippet
  HelloWorldGUI * m_GUI = new HelloWorldGUI();
  // EndCodeSnippet
  // 
  m_GUI->MainWindow->show();

  // BeginLatex
  // Next, we instantiate the ellipsoidal spatial object that we will be
  // attaching to the tracker:
  // EndLatex

  // Create the ellipsoid 
  // BeginCodeSnippet
  igstk::EllipsoidObject::Pointer ellipsoid = igstk::EllipsoidObject::New();
  // EndCodeSnippet
  // 
  // BeginLatex
  // The ellipsoid radius can be set to one in all dimensions ( X,Y, and Z ) 
  // using the \code{SetRadius} member function, as follows:
  // EndLatex
  // BeginCodeSnippet
  ellipsoid->SetRadius(1,1,1);
  // EndCodeSnippet

  // BeginLatex
  // To visualize the ellipsoid spatial object, an object representation 
  // class is created and the ellipsoid spatial object is added to it, 
  // as follows:
  // EndLatex
 
  // BeginCodeSnippet 
  igstk::EllipsoidObjectRepresentation::Pointer 
        ellipsoidRepresentation = igstk::EllipsoidObjectRepresentation::New();
  ellipsoidRepresentation->RequestSetEllipsoidObject( ellipsoid );
  ellipsoidRepresentation->SetColor(0.0,1.0,0.0);
  ellipsoidRepresentation->SetOpacity(1.0);
  // EndCodeSnippet
  
  // BeginLatex
  // Similarly, a cylinder spatial object and cylinder spatial object 
  // representation object are instantiated, as follows:
  // EndLatex

  
  // BeginCodeSnippet
  igstk::CylinderObject::Pointer cylinder = igstk::CylinderObject::New();
  cylinder->SetRadius(0.1);
  cylinder->SetHeight(3);

  igstk::CylinderObjectRepresentation::Pointer 
          cylinderRepresentation = igstk::CylinderObjectRepresentation::New();
  cylinderRepresentation->RequestSetCylinderObject( cylinder );
  cylinderRepresentation->SetColor(1.0,0.0,0.0);
  cylinderRepresentation->SetOpacity(1.0);
  // EndCodeSnippet
  
  // BeginLatex
  // Next, the spatial objects are added to the view, and the camera position
  // is reset to observe all objects in the scene, as follows:
  //
  // 
  // EndLatex
  // BeginCodeSnippet
  m_GUI->Display->RequestAddObject( ellipsoidRepresentation );
  m_GUI->Display->RequestAddObject( cylinderRepresentation );
  m_GUI->Display->RequestResetCamera();
  m_GUI->Display->Update();
  // EndCodeSnippet

  // BeginLatex
  // Function \code{RequestEnableInteractions()} allows the user to 
  // interactively manipulate (rotate, pan, zoomm etc.) the camera. 
  // For \doxygen{View2D} class, \code{vtkInteractorStyleImage} 
  // is used; for \doxygen{View3D} class, 
  // \code{vtkInteractorStyleTrackballCamera} is used. In IGSTK, the keyboard
  // events are disabled, so the original VTK 
  // key-mouse-combined interactions are not supported.
  // In summary, the mouse events are as follows: left button click triggers 
  // pick event; left button hold rotates the camera, in \doxygen{View3D}
  // (in \doxygen{View2D},
  // camera direction is always perpendicular to image plane, so there is no 
  // rotational movement available for it); middle mouse button 
  // pans the camera; right mouse button dollies the camera.
  // EndLatex
  // BeginCodeSnippet
  m_GUI->Display->RequestEnableInteractions();
  // EndCodeSnippet

  // BeginLatex
  // The following code instantiates a new mouse tracker and initializes it.
  // The scale factor is just a number to scale down the movement of the tracked
  // object in the scene:
  // EndLatex
  // BeginCodeSnippet
  igstk::MouseTracker::Pointer tracker = igstk::MouseTracker::New();
  tracker->Open();
  tracker->Initialize();
  tracker->SetScaleFactor( 100.0 );
  // EndCodeSnippet

  // BeginLatex
  // Now, The previously created spatial object is attached to the tracker and 
  // the tracker is set to monitor the mouse events from the user interface.
  // The tool port and tool number is the naming convention from NDI trackers 
  // (refer to Chapter \ref{Chapter:Tracker} on page \pageref{Chapter:Tracker}).
  // EndLatex
  // BeginCodeSnippet
  const unsigned int toolPort = 0;
  const unsigned int toolNumber = 0;
  tracker->AttachObjectToTrackerTool( toolPort, toolNumber, ellipsoid );
  m_GUI->SetTracker( tracker );
  // EndCodeSnippet

  // BeginLatex
  // Now a logger is set up. The log output is directed to both 
  // the standard output (std::cout) and a file (log.txt).
  // For the usage of priority level, please refer to Chapter 
  // \ref{Chapter:Logging} on page \pageref{Chapter:Logging}.
  // EndLatex
  // BeginCodeSnippet
  itk::Logger::Pointer logger = itk::Logger::New();
  itk::StdStreamLogOutput::Pointer logOutput = itk::StdStreamLogOutput::New();
  itk::StdStreamLogOutput::Pointer fileOutput = itk::StdStreamLogOutput::New();
    
  logOutput->SetStream( std::cout );
  logger->AddLogOutput( logOutput );
  logger->SetPriorityLevel( itk::Logger::DEBUG );

  std::ofstream ofs( "log.txt" );
  fileOutput->SetStream( ofs );
  logger->AddLogOutput( fileOutput );
  // EndCodeSnippet
  
  // BeginLatex
  // By connecting the logger to the Display and the Tracker, messages from 
  // these components are redirected to the logger, as follows: 
  // EndLatex
  // BeginCodeSnippet
  m_GUI->Display->SetLogger( logger ); 
  tracker->SetLogger( logger );
  // EndCodeSnippet


  // BeginLatex
  // Next, the refresh frequency of the display window is set. After the
  // \code{RequestStart()} function is called, the pulse generator inside the
  // display window will start ticking, and will call the display to update
  // itself 60 times per second, as follows: EndLatex
  // BeginCodeSnippet
  m_GUI->Display->RequestSetRefreshRate( 60 ); 
  m_GUI->Display->RequestStart();
  // EndCodeSnippet

  // BeginLatex
  // All applications should includes the following code. This is the main event
  // loop of the application. First, it checks if the application is aborted by
  // user. If not, it calls for the \doxygen{PulseGenerator} to check its time
  // out. The command is as follows:
  // EndLatex
  // BeginCodeSnippet
  while( !m_GUI->HasQuitted() )
    {
    Fl::wait(0.001);
    igstk::PulseGenerator::CheckTimeouts();
    }
  // EndCodeSnippet


  // BeginLatex
  // Finally, before exiting the application, the tracker is properly 
  // closed and other clean up procedures are executed, as follows:
  // EndLatex
  // BeginCodeSnippet
  tracker->StopTracking();
  tracker->Close();
  delete m_GUI;
  ofs.close();
  return EXIT_SUCCESS;
  // EndCodeSnippet

}
