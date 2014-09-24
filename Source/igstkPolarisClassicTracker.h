/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: igstkPolarisClassicTracker.h,v $
  Language:  C++
  Date:      $Date: 2008/02/11 01:41:51 $
  Version:   $Revision: 1.11 $

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __igstkPolarisClassicTracker_h
#define __igstkPolarisClassicTracker_h


#include "igstk_export.h"

#include "igstkSerialCommunication.h"
#include "igstkNDICommandInterpreterClassic.h"
#include "igstkTrackerTool.h"
#include "igstkNDITracker.h"
#include "igstkPolarisTrackerTool.h"

namespace igstk
{
/** \class PolarisClassicTracker
  * \brief Provides support for the Polaris optical tracker.
  *
  * This class provides an interface to the Classic Polaris.
  * This devices is manufactured by
  * Northern Digital Inc. of Waterloo, Ontario, Canada.
  *
  * \ingroup Tracker
  *PolarisClassicTracker
  */
class IGSTK_EXPORT PolarisClassicTracker : public NDITracker
{
public:
  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( PolarisClassicTracker, Tracker )
public:

  /** communication type */
  typedef igstk::SerialCommunication     CommunicationType;

  /** The SetCommunication method is used to attach a communication
    * object to the tracker object. */
  void SetCommunication( CommunicationType *communication );

protected:

  PolarisClassicTracker(void);

  virtual ~PolarisClassicTracker(void);

  /** typedef for command interpreter */
  typedef igstk::NDICommandInterpreterClassic   CommandInterpreterType;

  /** typedef for internal boolean return type */
  typedef Tracker::ResultType   ResultType;

  /** Open communication with the tracking device. */
  virtual ResultType InternalOpen( void );

  /** Close communication with the tracking device. */
  virtual ResultType InternalClose( void );

  /** Put the tracking device into tracking mode. */
  virtual ResultType InternalStartTracking( void );

  /** Take the tracking device out of tracking mode. */
  virtual ResultType InternalStopTracking( void );

  /** Update the status and the transforms for all TrackerTools. */
  virtual ResultType InternalUpdateStatus( void );

  /** This function is called by a separate thread. */
  virtual ResultType InternalThreadedUpdateStatus( void );

  /** Reset the tracking device to put it back to its original state. */
  virtual ResultType InternalReset( void );

  /** Verify tracker tool information */
  virtual ResultType VerifyTrackerToolInformation( const TrackerToolType * );

  /** Remove tracker tool entry from internal containers */
  virtual ResultType RemoveTrackerToolFromInternalDataContainers(
                                     const TrackerToolType * trackerTool );

  /** Add tracker tool entry to internal containers */
  virtual ResultType AddTrackerToolToInternalDataContainers(
                                    const TrackerToolType * trackerTool );

  /** Print object information */
  virtual void PrintSelf( std::ostream& os, ::itk::Indent indent ) const;

  /** Get method for the command interpreter
    * This will method will be used by the derived classes */
  CommandInterpreterType::Pointer GetCommandInterpreter() const;

  /** Helper function for reporting interpreter errors. */
  ResultType CheckError( CommandInterpreterType * ) const;

  /** Set port handle to be added */
  igstkSetMacro( PortHandleToBeAdded, int );

private:

  PolarisClassicTracker(const Self&);   //purposely not implemented
  void operator=(const Self&);   //purposely not implemented

  /** A mutex for multithreaded access to the buffer arrays */
  ::itk::MutexLock::Pointer  m_BufferLock;

  /** The "Communication" instance */
  CommunicationType::Pointer       m_Communication;

  /** The baud rate to use */
  CommunicationType::BaudRateType  m_BaudRate;

  /** The command interpreter */
  CommandInterpreterType::Pointer  m_CommandInterpreter;

  /** Port handle container indexed by the tracker tool unique
   * identifier */
  typedef std::string                           PortIdentifierType;
  typedef std::map< PortIdentifierType, int >   PortHandleContainerType;
  PortHandleContainerType                       m_PortHandleContainer;

  /** Container holding absent status of tools */
  typedef std::map< std::string, int >   ToolAbsentStatusContainerType;
  ToolAbsentStatusContainerType          m_ToolAbsentStatusContainer;

  /** Container holding status of the tools */
  typedef std::map< std::string, int >   ToolStatusContainerType;
  ToolStatusContainerType          m_ToolStatusContainer;

  /** A buffer to hold tool transforms */
  typedef std::vector < double >   InternalTransformType;
  typedef std::map< std::string , InternalTransformType >
                                TrackerToolTransformContainerType;

  TrackerToolTransformContainerType     m_ToolTransformBuffer;

  /** Port handle of tracker tool to be added */
  int m_PortHandleToBeAdded;
};

}

#endif //__igstk_PolarisClassicTracker_h_
