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

//: *************************************************************
//:	OPTOB1.CPP
//:	2/12/94
//: *************************************************************

#include <optob1.h>

// --------------------------------------------------------------
optoB1::optoB1(OMUXDEV omuxdev,GFSerial * serio,UWORD cfg,UWORD tres,\
UWORD latchpos,UWORD cntrpos)
:optomuxComm(omuxdev,serio,cfg) {
	
	lpos = latchpos;
	cpos = cntrpos;
	timer_resolution = tres;
	latch = dio = 0;
	if ( !isInvalid ) {
		SetTimerRes(tres);
		SetLatchEdges(lpos);
		ClearLatches();
		StartCounters(cpos);
	}
}

// --------------------------------------------------------------

optoB1::~optoB1(void) {
}

// --------------------------------------------------------------
BOOL optoB1::ResetBBconfig(void) {

	optomuxComm::ResetConfig();
	if ( !isInvalid ) {
		SetTimerRes(timer_resolution);
		SetLatchEdges(lpos);
		ClearLatches();
		StartCounters(cpos);
		return ClearCounters(cpos);
	}
	return(FALSE);
}

// --------------------------------------------------------------
BOOL optoB1::SetTimerRes(UWORD res) {

	SendCmd('n',0,0,res);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::SetWatchDog(UWORD data) {

	SendCmd('D',0,0,data);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ChanOn(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (FALSE);
	}
	SendCmd('K',(1<<chan),0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ChanOff(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (FALSE);
	}
	SendCmd('L',(1<<chan),0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::WriteMask(UWORD pos) {

	SendCmd('J',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ReadDIO(void) {
	
	SendCmd('M',0,0,0);
	BOOL rtn = CheckData();
	if ( rtn  ) {
		dio = DecodeWord();
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB1::SetLatchEdges(UWORD pos) {

	SendCmd('N',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ReadClearLatches(void) {

	BOOL rtn = ReadLatches();
	if ( rtn ) {
		ClearLatches(lpos);
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB1::ReadLatches(void) {
	
	SendCmd('Q',0,0,0);
	BOOL rtn = CheckData();
	if ( rtn  ) {
		latch = DecodeWord() & lpos;
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB1::ClearLatches(UWORD pos) {

	SendCmd('S',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::StartCounters(UWORD pos) {

	SendCmd('U',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::StopCounters(UWORD pos) {

	SendCmd('V',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ClearCounters(UWORD pos) {

	SendCmd('Y',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB1::ReadCounters(UWORD pos) {
	
	SendCmd('W',pos,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		DecodeIntoArray(pos);
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB1::ReadClearCntrs(UWORD pos) {

	SendCmd('X',pos,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		DecodeIntoArray(pos);
	}
	return(rtn);
}

// --------------------------------------------------------------
UWORD optoB1::GetCounter(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (0);
	}
	return(data[chan]);
}

// --------------------------------------------------------------
BOOL optoB1::GetLatch(UWORD chan) {
	
	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return FALSE;
	}
	return( (latch & (1 << chan)) ? TRUE : FALSE );
}

// --------------------------------------------------------------
BOOL optoB1::GetDIChannel(UWORD chan) {
	
	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return FALSE;
	}
	UWORD mask = dio & (~config);
	return( (mask & (1 << chan)) ? TRUE : FALSE );
}

// --------------------------------------------------------------
BOOL optoB1::GetDOChannel(UWORD chan) {
	
	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return FALSE;
	}
	UWORD mask = dio & config;
	return( (mask & (1 << chan)) ? TRUE : FALSE );
}

// --------------------------------------------------------------
UWORD optoB1::GetBinaryCode(UWORD msb,UWORD lsb) {

	UWORD j,mask;

	if ( (lsb < 0) || (msb > NUMB_CHAN-1) ) {
 		return (0);
	}
	if ( lsb > msb ) {
		return (0);
	}
	for (mask=0,j=lsb; j<=msb; j++) {
		mask |= (1 << j);
	}
	return( latch & mask );
}

//  ******************** end of file ****************************

