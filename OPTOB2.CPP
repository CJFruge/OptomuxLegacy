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
//:   OPTOB2.CPP  VERSION 4.00
//:   base class methods for optoB2
//: *************************************************************

// --------------------------------------------------------------
// -- includes --------------------------------------------------
// --------------------------------------------------------------

#include <optob2.h>

// --------------------------------------------------------------
optoB2::optoB2(OMUXDEV omuxdev,ACHANSTRUCT * ACScfg,\
GFSerial * serio,UWORD cfg,UWORD avgpos,UWORD nsamples)
:optomuxComm(omuxdev,serio,cfg) {

	achcfg = &b2cfg;
	if ( !ACScfg ) {
		for (int j=0; j<NUMB_CHAN; j++) {
			achcfg->ach[j].min 	= achcfg->ach[j].lo = 0;
			achcfg->ach[j].max 	= achcfg->ach[j].hi = 4095;
			achcfg->ach[j].stat	= TRUE;
			achcfg->ach[j].type	= TYPE_AD3;
			achcfg->ach[j].range	= FOUR_20_MA;
			achcfg->ach[j].maxaval= 20.0f;
			achcfg->ach[j].minaval= 4.0f;
			achcfg->ach[j].gain = 1.0f;
			achcfg->ach[j].cc = 1.0f;
			achcfg->ach[j].offset = 0.0f;
		}
		achcfg->samples = nsamples;
		achcfg->cfg = cfg;
		achcfg->avgpos = avgpos;
	}
	else {
		memcpy(achcfg,ACScfg,sizeof(ACHANSTRUCT));
		SetConfig(achcfg->cfg);
	}
	if ( !isInvalid && achcfg->avgpos ) {
		StartInputAvg(achcfg->avgpos,achcfg->samples);
	}
}

// --------------------------------------------------------------
optoB2::~optoB2(void) {
}

 
// --------------------------------------------------------------
BOOL optoB2::ResetBBconfig(void) {

	optomuxComm::ResetConfig();
	if ( !GetStatus() ) {
		SetConfig(achcfg->cfg);
		if ( achcfg->avgpos ) {
			return StartInputAvg(achcfg->avgpos,achcfg->samples);
		}
		return( GetConfig() == achcfg->cfg );
	}
	return (FALSE);
}
		
// --------------------------------------------------------------
BOOL optoB2::ReadAnalogIn(UWORD pos) {

	SendCmd('L',pos,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		DecodeIntoArray(pos);
	}
	MassageAnalog(pos);
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB2::ReadClearAvg(UWORD pos) {

	SendCmd('U',pos,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		DecodeIntoArray(pos);
	}
	MassageAnalog(pos);
	StartInputAvg(pos,achcfg->samples);
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB2::ReadAnalogAvg(UWORD pos) {

	SendCmd('U',pos,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		DecodeIntoArray(pos);
	}
	MassageAnalog(pos);
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB2::StartInputAvg(UWORD pos,UWORD avg) {

	SendCmd('T',pos,0,avg);
	return(CheckData());
}

// --------------------------------------------------------------
UWORD optoB2::GetCompBits(void) {

	SendCmd('i',0,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		rtn = DecodeWord() & achcfg->avgpos;
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optoB2::SetTempProbe(UWORD pos,UWORD type)	{

	SendCmd('k',pos,0,type);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optoB2::WriteOutputs(UWORD pos,UWORD val) {

	SendCmd('J',pos,0,val);
	return(CheckData());
}

// --------------------------------------------------------------
WORD optoB2::GetAchan(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (0);
	}
	return (data[chan]);
}

// --------------------------------------------------------------
WORD optoB2::GetAchanStat(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (0);
	}
	return (achcfg->ach[chan].stat);
}

// --------------------------------------------------------------
ACHAN * optoB2::GetAchanInfo(UWORD chan) {

	if ( (chan < 0) || (chan > NUMB_CHAN-1) ) {
 		return (0);
	}
	return (&(achcfg->ach[chan]));
}

// --------------------------------------------------------------
void optoB2::MassageAnalog(UWORD pos) {

	for (int j=0; j<NUMB_CHAN; j++) {
		if ( (pos & (1 << j)) ){
			data[j] -= 0x1000;
			if ( (data[j] < 0) || (data[j] > 0x1000) ) {
				achcfg->ach[j].stat = FALSE;
			}
		}
	}
}
	
//  *********************** end of file *************************

