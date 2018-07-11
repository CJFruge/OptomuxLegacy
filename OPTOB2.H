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
//:   OPTOB2.H  VERSION 4.00
//:   class declarations for optomuxComm classes
//: *************************************************************

#ifndef	OPTOB2_H_
#define	OPTOB2_H_

#include <optocom.h>

// input module ranges
#define	FOUR_20_MA	 	100
#define	ZERO_5_VOLT	 	101
#define	ZERO_10_VOLT 	102

// input modules
#define	TYPE_AD3			10
#define	TYPE_AD6			11
#define	TYPE_AD5			12
#define	TYPE_AD8			13
#define	TYPE_AD17		14
#define	TYPE_AD18		15
#define	TYPE_AD7			16
#define	TYPE_AD9			17
#define	TYPE_AD11		18
#define	TYPE_AD12		19
#define	TYPE_AD13		20
#define	TYPE_AD10T2		21
#define	TYPE_AD16T		22
#define	TYPE_AD4			23
#define	TYPE_AD15		24
#define	TYEP_AD20		25

// output modules
#define	TYPE_DA3			40
#define	TYPE_DA4			41
#define	TYPE_DA5			42
#define	TYPE_DA6			43
#define	TYPE_DA7			44
#define	TYPE_DA8			45

// --------------------------------------------------------------
//		max channels on b2 device
// --------------------------------------------------------------

typedef struct tagACHAN {

	WORD		range;
	float		maxaval;
	float		minaval;
	float		gain;
	float		offset;
	float		cc;

	WORD		min;
	WORD		max;
	WORD		hi;
	WORD		lo;
	WORD		stat;
	UWORD		type;
}
	ACHAN;
	
typedef struct tagACHANSTRUCT {

	UWORD 	cfg;
	UWORD		samples;
	UWORD		avgpos;
	ACHAN		ach[NUMB_CHAN];
}
	ACHANSTRUCT;

// --------------------------------------------------------------
//		optob2 class
// --------------------------------------------------------------

class optoB2 : public optomuxComm {

	ACHANSTRUCT	b2cfg;
	ACHANSTRUCT	* achcfg;

protected:
	void	MassageAnalog		(UWORD);	
public:
	optoB2(OMUXDEV,
			ACHANSTRUCT *,
			GFSerial *,
			UWORD cfg=0x0,
			UWORD avgpos=0xFFFF,
			UWORD nsamples = 6);
	~optoB2(void);
	// redef
	
	// command set
	BOOL	ResetBBconfig	(void);
	BOOL	ReadAnalogIn	(UWORD);
	BOOL	ReadAnalogAvg	(UWORD);
	BOOL	ReadAnalogAvg	(void) {
		return ReadAnalogAvg(achcfg->avgpos);
	}
	BOOL	ReadClearAvg	(UWORD);
	BOOL	ReadClearAvg	(void) {
		return ReadClearAvg(achcfg->avgpos);
	}
	BOOL	StartInputAvg	(UWORD,UWORD);
	BOOL	StartInputAvg	(void) {
		return StartInputAvg(achcfg->avgpos,achcfg->samples);
	}
	BOOL	SetTempProbe	(UWORD,UWORD);
	BOOL	WriteOutputs	(UWORD,UWORD);
	UWORD	GetCompBits		(void);	
	// accessor
	WORD	GetAchanStat	(UWORD);
	WORD	GetAchan			(UWORD);
	ACHAN	*GetAchanInfo	(UWORD);
};

#endif	// OPTOB2_H_

//: *********************** end of file *************************


