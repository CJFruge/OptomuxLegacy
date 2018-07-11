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
//:   OPTOCOM.H  VERSION 4.00
//:   class declarations for optomuxComm classes
//: *************************************************************

// --------------------------------------------------------------
// -- includes --------------------------------------------------
// --------------------------------------------------------------

#ifndef	OPTOCOM_H_
#define	OPTOCOM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <compplib.hpp>
#include <ibmkeys.h>
#include <psmax.h>

#define	NUMB_POS		4
#define	NUMB_CHAN		16

#define GF_CRLF         -3
#define GF_LF           -2
#define GF_NULL         -1

typedef	unsigned int	UWORD;
typedef	unsigned int	OMUXDEV;

#ifdef	__TURBOC__
#define	_ltoa				ltoa;
#endif

// --------------------------------------------------------------
// -- optomux classes -------------------------------------------
// --------------------------------------------------------------

class optomuxComm {

private:
	char				cmd[256];
	char				* pcmd;
	char				cbuff[256];
	char				* pcbuff;
	GFSerial 		* sio;
	GFLineStatus 	* ls;
	GFDataFormat	* df;
	char				rbuff[256];

protected:
	BOOL 	SetConfig(UWORD);
	BOOL	PUClear	(void);
	BOOL	Reset		(void);
	void	DecodeIntoArray(UWORD);
	UWORD	DecodeWord		(void);

	// class data
	char				* prbuff;
	WORD				data[NUMB_CHAN];
	OMUXDEV			dev;
	UWORD				rxlen;
	int				txlen;
	BOOL				isInvalid;
	UWORD				config;

	// string handling command functions
	void	SendCmd			(char,UWORD,UWORD,UWORD);
	BOOL	CheckData		(void);

	// shared optomux funcs
public:
	virtual	void	ResetConfig		(void);
				BOOL	GetStatus		(void);
				BOOL	SetTurnAround	(char);
				BOOL	SetProtocol		(char);
				UWORD	Identify			(void);
				UWORD	GetConfig		(void);

	// constructor,destructor
protected:
	optomuxComm				(OMUXDEV,GFSerial *,UWORD cfg=0x0);
	~optomuxComm			(void);
};

#endif	// OPTOCOM_H_
//  ************************ end of file ************************

