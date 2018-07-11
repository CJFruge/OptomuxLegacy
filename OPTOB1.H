/****************************************************************
*****************************************************************
**
**  Copyright (c) 1993
**  ----------------------------------
**  P E T R O S O F T, INC.
**  1822 Barker-Cypress Road, No. 303
**  Houston, TX 77084-4565
**  713-579-7165
**  ----------------------------------
**  by:
**
**  CJ Fruge
**
**  All rights reserved.
**
**  Software & systems engineering by CJ Fruge
**
**  06/19/93
**
*****************************************************************
****************************************************************/

#ifndef OPTOB1_H_
#define OPTOB1_H_

#include <optocom.h>

//: *************************************************************
//:   OPTOB1.H  VERSION 4.00
//:   class declarations for optomuxComm classes
//: *************************************************************

class optoB1:public optomuxComm {

	UWORD		dio,latch,lpos,cpos;
   UWORD		timer_resolution;

public:
	optoB1(OMUXDEV omuxdev,
			GFSerial * serio,
			UWORD cfg		= 0x0000,
			UWORD tres 		= 2,
			UWORD latchpos = 0x0000,
			UWORD cntrpos 	= 0x0000);
	~optoB1(void);

	// command functions
	BOOL	ResetBBconfig	(void);
	BOOL	SetTimerRes		(UWORD);
	BOOL	SetTimerRes		(void) {
		return SetTimerRes(timer_resolution);
	}
	BOOL	SetWatchDog		(UWORD);
	BOOL	WriteMask		(UWORD);
	BOOL	ChanOn			(UWORD);
	BOOL	ChanOff			(UWORD);
	BOOL	ReadDIO			(void);
	
	BOOL	SetLatchEdges	(UWORD);
	BOOL	SetLatchEdges	(void) {
		return SetLatchEdges(lpos);
	}
	BOOL	ClearLatches	(UWORD);
	BOOL	ClearLatches	(void) {
		return ClearLatches(lpos);
	}
	BOOL	StartCounters	(UWORD);
	BOOL	StartCounters	(void) {
		return StartCounters(cpos);
	}
	BOOL	StopCounters	(UWORD);
	BOOL	StopCounters	(void) {
		return StopCounters(cpos);
	}
	BOOL	ClearCounters	(UWORD);
	BOOL	ClearCounters	(void) {
		return ClearCounters(cpos);
	}
	// stuffs data array
	BOOL	ReadCounters	(UWORD);
	BOOL	ReadCounters	(void) {
		return ReadCounters(cpos);
	}
	BOOL	ReadClearCntrs	(UWORD);
	BOOL	ReadClearCntrs	(void) {
		return ReadClearCntrs(cpos);
	}
	BOOL	ReadClearLatches(void);
	BOOL	ReadLatches		(void);
   
	// data accessors
	UWORD	GetCounter		(UWORD);
	BOOL	GetLatch			(UWORD);
 	BOOL	GetDOChannel	(UWORD);
 	BOOL	GetDIChannel	(UWORD);
   UWORD	GetBinaryCode	(UWORD,UWORD);
	UWORD	GetLatchWord	(void) {
		return latch;
	}
	UWORD	GetDIWord		(void) {
		return dio & (~config);
	}
	UWORD	GetDOWord		(void) {
		return dio & config;
	}

	// config accessors
	UWORD	GetCntrConfig	(void) {
		return cpos;
	}
	UWORD	GetLatchConfig	(void) {
		return lpos;
	}
};

#endif // OPTOB1_H_

//: *********************** end of file *************************

