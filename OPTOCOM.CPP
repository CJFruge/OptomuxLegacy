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
//:   OPTOCOM.CPP  VERSION 4.00
//:   base class methods for optomuxComm
//: *************************************************************

// --------------------------------------------------------------
// -- includes --------------------------------------------------
// --------------------------------------------------------------

#include <optocom.h>

// --------------------------------------------------------------
// optoComm constructor
// --------------------------------------------------------------
optomuxComm::optomuxComm(OMUXDEV omuxdev,GFSerial * serio,UWORD cfg) {

	dev = omuxdev;
	sio = serio;
	ls = sio->GetLineStatus();
	df = sio->GetDataFormat();
	pcmd	 = &cmd[0];
	pcbuff = &cbuff[0];
	prbuff = &rbuff[0];
	config = cfg;
	if ( !GetStatus() ) {
		PUClear();
		Reset();
		isInvalid = !SetConfig(config);
	}
	for (int j=0; j<NUMB_CHAN; j++) {
		data[j]=0;
	}
}

// --------------------------------------------------------------
optomuxComm::~optomuxComm(void) {
}

// --------------------------------------------------------------
BOOL optomuxComm::CheckData(void) {

	char	* psb = prbuff;
	char	* pse;
	int	cksum;

	if ( (*psb == 'A') && (strlen(psb) == 2) ) {
		return(TRUE);
	}
	if ( *psb == 'N' ) {
		return (FALSE);
	}	
	for (psb = prbuff; (*psb != '\n'&& *psb != '\0'); psb++ ) {
		;
	}
	pse = psb;		// end of string (pse p2 '\n')
	pse -= 3;		// back up to beginning of cksum
	psb = prbuff;	// rewind
	psb++;			// bump past 'A'
	for (cksum=0; psb < pse; psb++) {
		cksum += *psb;
	}
	cksum %= 256;
	sprintf(pcbuff,"%02X",cksum);
	if ( !(strncmp(pcbuff,pse,2)) ) {
		return (TRUE);
	}
	return(FALSE);
}

// --------------------------------------------------------------
BOOL optomuxComm::PUClear(void) {

	SendCmd('A',0,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optomuxComm::Reset(void) {
	
	SendCmd('B',0,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optomuxComm::SetTurnAround(char delay) {

	SendCmd('C',0,0,delay);
	return(CheckData());
}

// --------------------------------------------------------------
BOOL optomuxComm::SetProtocol(char prot) {

	SendCmd('E',0,0,prot);
	return(CheckData());
}

// --------------------------------------------------------------
UWORD optomuxComm::Identify(void) {
	
	SendCmd('F',0,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		rtn = DecodeWord();
	}
	return (rtn = FALSE ? 0x7fff : rtn);
}

// --------------------------------------------------------------
UWORD optomuxComm::GetConfig(void) {
	
	SendCmd('j',0,0,0);
	UWORD rtn = CheckData();
	if ( rtn ) {
		rtn = DecodeWord();
	}
	return(rtn);
}

// --------------------------------------------------------------
BOOL optomuxComm::SetConfig(UWORD pos) {
	
	SendCmd('G',pos,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
void optomuxComm::SendCmd(char cl,UWORD pos,UWORD mod,UWORD data) {
	
	int 	len,cksum;
	char 	fmt[32], * ps;
	BOOL	isPos;

	len = sprintf(pcmd,">%02X%c",dev,cl);
	switch (cl) {
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'M':
		case 'O':
		case 'Q':
		case 'd':
		case 'j':
		case 'i':
		case 'n':
			isPos = FALSE;
 			break;
		default:
			isPos = TRUE;
			break;
	}
	if ( isPos ) {
		pos &= 0xFFFF;
		len = sprintf(fmt,"%04X",pos);
		strncat(pcmd,fmt,len);
	}
	if ( mod ) {
		len = sprintf(fmt,"%X",mod);
		strncat(pcmd,fmt,len);
	}
	if ( data ) {
		len = sprintf(fmt,"%02X",data);
		strncat(pcmd,fmt,len);
	}
	for(ps=pcmd+1,cksum=0; *ps != '\0'; ps++) {
	  	cksum += *ps;
	}
	cksum %= 256;
	sprintf(pcbuff,"%02X",cksum);
	strncat(pcmd,pcbuff,2);
	txlen = sio->PutString((const BYTE *)pcmd,CR);
	rxlen = sio->GetString((BYTE *)prbuff,sizeof(rbuff),CR,200);
}	

// --------------------------------------------------------------
UWORD	optomuxComm::DecodeWord(void) {
	
	char  * psb = prbuff;
	char	s[8];
	
	if ( *psb == 'N' ) {
		return(0);
	}
	psb++;
	for (int j=1; j<sizeof(s); j++) s[j]='\0';
	strncpy(s,psb,4);
	return ((UWORD) strtol(s,0,16));
}

// --------------------------------------------------------------
void optomuxComm::ResetConfig (void) {

	if ( !GetStatus() ) {
		PUClear();
		Reset();
		isInvalid = !SetConfig(config);
	}
	else {
		isInvalid = FALSE;
	}
}
// --------------------------------------------------------------
BOOL optomuxComm::GetStatus (void) {

	SendCmd('F',0,0,0);
	return(CheckData());
}

// --------------------------------------------------------------
void optomuxComm::DecodeIntoArray(UWORD pos) {

	int 	j;
	char  * psb = prbuff;
	char	spos[8];
	char	* s = &spos[0];

	for (j=1;j<sizeof(spos);j++) spos[j] = '\0';
	psb++;
	for (j=NUMB_CHAN-1; j>=0; j--) {
		if ( (pos & (1 << j)) ) {
			strncpy(s,psb,4);
			psb+=4;
			data[j] = (UWORD) strtol(s,0,16);
		}
	}	
}

// *********************** end  of file *************************

