/*
Lendal the automated Length Value pair finder in arbitrary data

Released as open source by NCC Group Plc - http://www.nccgroup.com/

Developed by Ollie Whitehouse, ollie dot whitehouse at nccgroup dot com

Originally developed in 2006

http://www.github.com/nccgroup/lendal

Released under AGPL see LICENSE for more information
*/

#include "stdafx.h"
#include "getopt.h"

// -------------------------------------------------------------------------
// Function: Banner
// Description:	Print banner
// -------------------------------------------------------------------------
void banner(){
	fprintf(stdout,"-------------------------------------------------------------------------\n");
	fprintf(stdout,"Len - 'Lendal' the length finder - v0.2\n");
	fprintf(stdout,"Released as open source by NCC Group Plc - http://www.nccgroup.com/\n");
	fprintf(stdout,"-------------------------------------------------------------------------\n\n");
}

// -------------------------------------------------------------------------
// Function: Banner End
// Description:	Print shutdown banner
// -------------------------------------------------------------------------
void bannerstop(){
	fprintf(stdout,"\n-------------------------------------------------------------------------\n");
	fprintf(stdout,"NCC Group\n");
	fprintf(stdout,"-------------------------------------------------------------------------\n\n");
}

// -------------------------------------------------------------------------
// Function: Banner Help
// Description:	Print help
// -------------------------------------------------------------------------
void bannerhelp(_TCHAR *strProgname){
	fprintf(stdout,"\n");
	fprintf(stdout,"%s -i <filename>\n",strProgname);
	fprintf(stdout,"example: %s -i ollie.doc\n\n",strProgname);
	fprintf(stdout,"[options]\n");
	fprintf(stdout,"  -i <filename> Input file\n");
	fprintf(stdout,"  -g <number> Gap between len and data (in bytes)\n");
	fprintf(stdout,"  -m <number> Minimum length we worry about (in bytes)\n");
	fprintf(stdout,"  -s swap endian\n");
	fprintf(stdout,"  -u UNICODE mode\n");
	fprintf(stdout,"  -p <number> allow non printable chars (this many - 255 for no limit)\n");
	fprintf(stdout,"  -o Allow over laping len\n");
	fprintf(stdout,"  -n NONULLS mode (i.e. don't require a NULL terminator)\n");
	fprintf(stdout,"  -B BYTE mode(default)\n");
	fprintf(stdout,"  -W WORD mode\n");
	fprintf(stdout,"  -D DOUBLE mode\n");
	fprintf(stdout,"\n");
	fprintf(stdout,"  -h Help\n");
	fprintf(stdout,"\n");
}



// -------------------------------------------------------------------------
// Function		:	CheckSize
// Description	:	Checks we wont run out of space
// Takes		:	
// Returns		:	0 - no error
//				   >0 - error
// -------------------------------------------------------------------------
int CheckSize(unsigned int lngSize, unsigned int lngFileSize, unsigned int lngCurrPos, unsigned int intDebug, unsigned int intGAP){

	if(intDebug) fprintf(stdout,"[D] Size is %llu\n",lngSize);

	if (lngSize>lngFileSize){
		if(intDebug) fprintf(stdout,"[D] Len of %llu bytes is bigger than file size %lu\n",lngSize,lngFileSize);
		return 1;
	} else if(lngSize>(lngFileSize-lngCurrPos-intGAP)){
		if(intDebug) fprintf(stdout,"[D] Len of %llu bytes is bigger than the remaining %lu bytes\n",lngSize,(lngFileSize-lngCurrPos-intGAP));
		return 2;
	} else {
		return 0;
	}
}


int PrintSearchType(unsigned int intType){

	switch(intType){
		case 1: // byte
			fprintf(stdout,"[*] \t\tBYTE         : [X]\n");	
			fprintf(stdout,"[*] \t\tWORD         : [ ]\n");	
			fprintf(stdout,"[*] \t\tDWORD        : [ ]\n");	
			fprintf(stdout,"[*] \t\tUNKNOWN      : [ ]\n");	
			break;
		case 2: // word
			fprintf(stdout,"[*] \t\tBYTE         : [ ]\n");	
			fprintf(stdout,"[*] \t\tWORD         : [X]\n");	
			fprintf(stdout,"[*] \t\tDWORD        : [ ]\n");	
			fprintf(stdout,"[*] \t\tUNKNOWN      : [ ]\n");	
			break;
		case 4: // dword
			fprintf(stdout,"[*] \t\tBYTE         : [ ]\n");	
			fprintf(stdout,"[*] \t\tWORD         : [ ]\n");	
			fprintf(stdout,"[*] \t\tDWORD        : [X]\n");	
			fprintf(stdout,"[*] \t\tUNKNOWN      : [ ]\n");	
			break;
		default:
			fprintf(stdout,"[*] \t\tBYTE         : [ ]\n");	
			fprintf(stdout,"[*] \t\tWORD         : [ ]\n");	
			fprintf(stdout,"[*] \t\tDWORD        : [ ]\n");	
			fprintf(stdout,"[*] \t\tUNKNOWN      : [X]\n");	
			fprintf(stdout,"%u\n",intType);
	}

	return 0;
}

int PrintEndianType(unsigned int intType){

	switch(intType){
		case 1: // byte
			fprintf(stdout,"[*] \t\tENDIAN SWAP  : [ ]\n");	
			break;
		case 2: // word
			fprintf(stdout,"[*] \t\tENDIAN SWAP  : [X]\n");	
			break;
		default:
			fprintf(stdout,"[*] \t\tENDIAN SWAP  : [?]\n");	
	}

	return 0;
}
// -------------------------------------------------------------------------
// Function		:	PrintOutFinding
// Description	:	
// Takes		:	
// Returns		:	0 - no error
//				   >0 - error
// -------------------------------------------------------------------------
int PrintOutFinding(char *strWorkBuff, unsigned int intLenPos, unsigned int intLen, unsigned int intStrPos, unsigned int intPlusOne, int intNonPrintable, int intDebug,int intNextAdd, unsigned int intGAP, int intEndian,int intUC)
{
	unsigned int intCnt=0;
	int			 intNonPrintCharFound=0;
	int			 intCnt2=0;
	int			 intRet=0,intRet2=0;
	WCHAR		 *wszSrc;
	char		 *strDest; 
	int			 intTemp=0;

	if(intDebug)fprintf(stdout,"[D] PrintOutFinding - intStrPos %u\n",intStrPos);

	for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
		if ((strWorkBuff[intCnt]>=32) && (strWorkBuff[intCnt]<=127)){
				//
		} else {
			if ((strWorkBuff[intCnt]==0x00) && (intCnt == (intStrPos+intLen-1)))
			{
				
			} else {
				if(intDebug)fprintf(stdout,"[D] PrintOutFinding() %02X %u %u\n",strWorkBuff[intCnt]&0xFF,intStrPos,(intStrPos+intLen-1));
				intNonPrintCharFound=1;
				intCnt2++;
			}
		}
	}

	if(intNonPrintCharFound==0 && intNonPrintable==0){
		fprintf(stdout,"[*] Potential LENGTH finding:\n");
		fprintf(stdout,"[*] \t\tLength offset: %0.8X\n",intLenPos);
		fprintf(stdout,"[*] \t\tLength value : %u\n",intLen);
		fprintf(stdout,"[*] \t\tData value   : ",intLen);

		for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
			if (((strWorkBuff[intCnt]>=32) && (strWorkBuff[intCnt]<=127)) || strWorkBuff[intCnt]== 13 || strWorkBuff[intCnt]== 10 ){
				fprintf(stdout,"%c",strWorkBuff[intCnt]);
			} else {
				if(strWorkBuff[intCnt]==0x00){
					fprintf(stdout,"[NULL]");
				} else{
					fprintf(stdout,".");
				}
			}
		}

		fprintf(stdout,"\n");
	
		PrintSearchType(intNextAdd);
		PrintEndianType(intEndian);
		fprintf(stdout,"[*] \t\tUNICODE      : [ ]\n");

		if (intPlusOne){
			fprintf(stdout,"[*] \t\tNULL +1      : [X]\n");
		} else {
			fprintf(stdout,"[*] \t\tNULL +1      : [ ]\n");
		}

	} else if (intNonPrintable>0){
		// fprintf(stdout,"[D] intNonPrintable %d, intCnt2 %d\n",intNonPrintable, intCnt2);
		if(intNonPrintable==255){ // just print em
			fprintf(stdout,"[*] Potential finding:\n");
			fprintf(stdout,"[*] \t\tLength offset: %0.8X\n",intLenPos);
			fprintf(stdout,"[*] \t\tLength value : %u\n",intLen);
			fprintf(stdout,"[*] \t\tData value   : ",intLen);

			for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
				if ((strWorkBuff[intCnt]>=32) && (strWorkBuff[intCnt]<=127)){
					fprintf(stdout,"%c",strWorkBuff[intCnt]);
				} else {
					if(strWorkBuff[intCnt]==0x00){
						fprintf(stdout,"[NULL]");
					} else{
						fprintf(stdout,".");
					}
				}
			}

			fprintf(stdout,"\n");
			PrintSearchType(intNextAdd);
			PrintEndianType(intEndian);

		} else if (intCnt2 <= intNonPrintable){
			fprintf(stdout,"[*] Potential finding:\n");
			fprintf(stdout,"[*] \t\tLength offset: %0.8X\n",intLenPos);
			fprintf(stdout,"[*] \t\tLength value : %u\n",intLen);
			fprintf(stdout,"[*] \t\tData value   : ",intLen);

			for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
				if ((strWorkBuff[intCnt]>=32) && (strWorkBuff[intCnt]<=127)){
					fprintf(stdout,"%c",strWorkBuff[intCnt]);
				} else {
					if(strWorkBuff[intCnt]==0x00){
						fprintf(stdout,"[NULL]");
					} else{
						fprintf(stdout,".");
					}
				}
			}

			fprintf(stdout,"\n");
			PrintSearchType(intNextAdd);
			PrintEndianType(intEndian);
			fprintf(stdout,"[*] \t\tUNICODE      : [ ]\n");
		}
	} else if (intUC){
		wszSrc=(WCHAR *)malloc((intLen+10)*sizeof(WCHAR));
		strDest=(char *)malloc((intLen+10)*sizeof(char));
	
		// Some error checking here
		memset(wszSrc,0x00,(intLen+10)*sizeof(WCHAR));
		memset(strDest,0x00,(intLen+10)*sizeof(char));

		for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
			wszSrc[intTemp]=strWorkBuff[intCnt];			
			intTemp++;
		}
		fprintf(stdout,"%d",intTemp);

		


		//fwprintf(stdout,L"\n\n! %s !\n\n",wszSrc);

		intRet = NULL;
		intRet=IS_TEXT_UNICODE_UNICODE_MASK;
		intRet2=IS_TEXT_UNICODE_NOT_UNICODE_MASK;
		if(IsTextUnicode(wszSrc,intLen,&intRet2) == 0){
			intRet=0;
			//fprintf(stdout,"%d\n",intLen);
			intRet=WideCharToMultiByte( CP_ACP, 0, wszSrc, intTemp, strDest, intLen,NULL,NULL)>0;
			if(intRet>0)
			{
				//fprintf(stdout,"%d\n",intRet);
				fprintf(stdout,"[*] Potential LENGTH finding:\n");
				fprintf(stdout,"[*] \t\tLength offset: %0.8X\n",intLenPos);
				fprintf(stdout,"[*] \t\tLength value : %u\n",intLen);
				fprintf(stdout,"[*] \t\tData value   : ",intLen);

				for(intCnt=intStrPos;intCnt<=(intStrPos+intLen-1);intCnt++){
					if ((strWorkBuff[intCnt]>=32) && (strWorkBuff[intCnt]<=127)){
						fprintf(stdout,"%c",strWorkBuff[intCnt]);
					} else {
						if(strWorkBuff[intCnt]==0x00){
							//fprintf(stdout,"[NULL]");
						} else{
							fprintf(stdout,".");
						}
					}
				}

				fprintf(stdout,"\n");
				fprintf(stdout,"\n");

				for(intCnt=0;intCnt<(unsigned int)intRet;intCnt++){
					if ((strDest[intCnt]>=32) && (strDest[intCnt]<=127)){
						fprintf(stdout,"%c",strDest[intCnt]);
					} else {
						if(strDest[intCnt]==0x00){
							fprintf(stdout,"[NULL]");
						} else{
							fprintf(stdout,".");
						}
					}
				}

				fprintf(stdout,"\n");

				PrintSearchType(intNextAdd);
				PrintEndianType(intEndian);
				fprintf(stdout,"[*] \t\tUNICODE      : [X]\n");

				if (intPlusOne){
					fprintf(stdout,"[*] \t\tNULL +1      : [X]\n");
				} else {
					fprintf(stdout,"[*] \t\tNULL +1      : [ ]\n");
				}
			}
		} 
		free(wszSrc);
		free(strDest);
	}


	return 0;
}
// -------------------------------------------------------------------------
// Function		:	ChecksLens
// Description	:	Check after the DATA (or +1) is a NULL or is allowed to be overridden
//                  Call print out function if cool
// Takes		:	Pointer to buffer
//					Length of buffer
//					Find type
//					Debugging
//					Unicode mode
//					Endian mode
//					Gap Between LEN and data
//					NextAdd (used for getting to the data)
//					If we check for NULL termination
// Returns		:	0 - no error
//				   >0 - error
// -------------------------------------------------------------------------
int CheckLens(char *strWorkBuff, unsigned int intCurrPos,unsigned int intLen,int intUC, int intEndian, unsigned int intGAP, int intNextAdd, int intDebug,int intNoNulls,int intMinLen, int intNonPrintable)
{
	unsigned int intPos=0;

	if(intDebug)fprintf(stdout,"[D] CheckLens()\n");

	if(intDebug)fprintf(stdout,"[D] CheckLens() - NextAdd %u\n",intNextAdd);

	if(intDebug)fprintf(stdout,"[D] intCurrPos %u intNextAdd %u intGAP %u\n",intCurrPos,intNextAdd,intGAP);

	// Make sure we skip the length
	intPos = intCurrPos+intNextAdd+intGAP;
	if(intDebug)fprintf(stdout,"[D] intPos %u \n",intPos);
	
	// General debugging
	if(intDebug)fprintf(stdout,"[D] CheckLens() - Offset %u\n",intPos);
	if(intDebug)fprintf(stdout,"[D] CheckLens() Byte %02X\n",strWorkBuff[intPos+intLen]&0xFF);
	if(intDebug)fprintf(stdout,"[D] CheckLens() Byte %02X\n",strWorkBuff[intPos+intLen+1]&0xFF);
	if(intDebug)fprintf(stdout,"[D] * %u \n",intPos+intLen);


	// We now check if the value is NULL terminated
	if(strWorkBuff[intPos+intLen-1]==0x00){			// this as we are at the start of the sting
		PrintOutFinding(strWorkBuff, intCurrPos, intLen, intPos, 0, intNonPrintable,intDebug,intNextAdd,intGAP,intEndian,intUC);
	} else if(strWorkBuff[intPos+intLen]==0x00){	// 
		PrintOutFinding(strWorkBuff, intCurrPos, intLen, intPos, 1, intNonPrintable,intDebug,intNextAdd,intGAP,intEndian,intUC);
	} else {
		if(intDebug)fprintf(stdout,"[D] CheckLens() After Byte %02X\n",strWorkBuff[intPos+intLen]&0xFF);
		if(intNoNulls){
			PrintOutFinding(strWorkBuff, intCurrPos, intLen, intPos, 0,intNonPrintable,intDebug,intNextAdd,intGAP,intEndian,intUC);
		}
	}
	

	return 0;
}

// -------------------------------------------------------------------------
// Function		:	FindsLens
// Description	:	Finds lens
// Takes		:	Pointer to buffer
//					Length of buffer
//					Find type
//					Debugging
//					Unicode mode
//					Endian mode
// Returns		:	0 - no error
//				   >0 - error
// -------------------------------------------------------------------------
int FindLens(char *strWorkBuff, unsigned int lngFileSize,int intFindType, int intDebug, int intUC, int intEndian, unsigned int intGAP, int intNoNulls,unsigned int intMinLen, int intNonPrintable)
{
	unsigned int	lngCurrPos=0;
	int				intNextAdd=0;
	unsigned int	lngLen=0;

	unsigned int	lngTemp=0;
	unsigned int	lngTemp2=0;
	unsigned int	intRet;
		   
	
	// Work out the find type and tell how many bytes we will add
	// between each iteration
	switch (intFindType){
		case 1: // byte
			intNextAdd=1;			
			break;
		case 2: // word
			intNextAdd=2;
			break;
		case 3: // dword
			intNextAdd=4;
			break;
		default:
			return 99;
	}


	// Stop my coding mistakes
	if (intNextAdd == 0) return 99; 

	// This walks through the file
	// this checks that we wont overrun the buffer
	// This might lead to instances where the large oddly shaped word or dword won't get fuzzed
	while (lngCurrPos+intNextAdd-1<lngFileSize)
    {
		if (intDebug) fprintf(stdout,"[D] Find type is %d, File offset is %0.8X \n",intFindType,lngCurrPos);

		lngTemp=0;
		lngLen=0;

		switch (intFindType){
			case 1: // byte
				if(intDebug) fprintf(stdout,"[D] Byte %02X\n",strWorkBuff[lngCurrPos]&0xFF);
				lngTemp=(unsigned char)strWorkBuff[lngCurrPos];
		        lngTemp|=0x00 << 8;
				lngTemp|=0x00 << 16;
				lngTemp|=0x00 << 24;
				break;
			case 2: // double byte
				if(intEndian==1){
					if(intDebug) fprintf(stdout,"[D] Bytes %02X %02X\n",strWorkBuff[lngCurrPos]&0xFF,strWorkBuff[lngCurrPos+1]&0xFF);
					lngTemp =(unsigned char)strWorkBuff[lngCurrPos];
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+1] << 8;
					lngTemp|=0x00 << 16;
					lngTemp|=0x00 << 24;
				} else if (intEndian==2){
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+1];
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos] << 8;
					lngTemp|=0x00 << 16;
					lngTemp|=0x00 << 24;
				}
				break;
			case 3: // word
				if(intEndian==1){
					if(intDebug) fprintf(stdout,"[D] Bytes %02X %02X %02X %02X\n",strWorkBuff[lngCurrPos]&0xFF,strWorkBuff[lngCurrPos+1]&0xFF,strWorkBuff[lngCurrPos+2]&0xFF,strWorkBuff[lngCurrPos+3]&0xFF);
					lngTemp =(unsigned char)strWorkBuff[lngCurrPos];
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+1] << 8;
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+2] << 16;
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+3] << 24;
				} else if (intEndian==2){
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+3];
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+2] << 8;
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos+1] << 16;
					lngTemp|=(unsigned char)strWorkBuff[lngCurrPos] << 24;
				}
				break;
			default:
				return 99;

		}

		lngLen=lngTemp;
		if(CheckSize(lngLen,lngFileSize,lngCurrPos,intDebug,intGAP)==0){
			// so we are not going to run out of space
			// hand off to check for the ret
			if(lngLen>intMinLen){
				intRet=CheckLens(strWorkBuff, lngCurrPos, lngLen, intUC, intEndian, intGAP, intNextAdd,intDebug,intNoNulls,intMinLen, intNonPrintable);
				if(intRet>0){ // return an error
					return intRet;
				}
			}

		} else {
			// we could do something here

		}

		//lngCurrPos += intNextAdd;
		lngCurrPos += 1;
	}

	return 0;
}

// -------------------------------------------------------------------------
// Function		:	FindsLensWrapper
// Description	:	This is a wrapped to FindLens() to work on files
//					This is to make it easier to libraryize the file
// Takes		:	Filename
//					The find type
//					Debugging
//					Unicode mode
//					Endian mode
// Returns		:	0 - no error
//				   >0 - error
// -------------------------------------------------------------------------
int FindLensWrapper(char *strFile, int intFindType, int intDebug, int intUC, int intEndian, unsigned int intGAP, int intNoNulls, unsigned int intMinLen, int intNonPrintable)
{
	FILE	*fInFile;   
	
	char	*strWorkBuff;

	unsigned int		lngCurrPos=0;
	unsigned int		lngFileSize=0;
	unsigned int		lngNumofBytes;
	unsigned int		lngNumRead=0;
	int					intRet=0;
	errno_t				iError=0;
		   
	if(intDebug) fprintf(stdout,"[D] Input file %s\n",strFile);

	// Open the intput file
	iError = fopen_s(&fInFile,strFile,"rb"); 
	if (fInFile == NULL) return 1;

	// Get the file size
	fseek( fInFile, 0L, SEEK_END );
	lngFileSize = ftell( fInFile );
	if (intDebug) fprintf(stdout,"[D] Input file size %lu bytes\n",lngFileSize);

	lngNumofBytes = lngFileSize;
	
	if (intDebug) fprintf(stdout,"[D] Scanning %lu bytes of file\n",lngNumofBytes);
	fseek( fInFile, 0L, SEEK_SET );

	if (intDebug) fprintf(stdout,"[D] Number of bytes in file %lu \n",lngNumofBytes);
	
	// allocate, read the file into memory, close source
	strWorkBuff = (char *)malloc(lngFileSize);
	if (strWorkBuff == NULL) return 5;

    lngNumRead = fread( strWorkBuff, sizeof( char ), lngFileSize, fInFile);
	if (lngNumRead != lngFileSize){
		
		if (intDebug) fprintf(stdout,"[D] Source file size %lu, bytes read %lu\n",lngFileSize,lngNumRead);
		free(strWorkBuff);
		return 6;
	} else {
		if (intDebug) fprintf(stdout,"[D] Source file size %lu, bytes read %lu\n",lngFileSize,lngNumRead);
	}
	fclose(fInFile);

	// Hand off to the work
	intRet=FindLens(strWorkBuff,lngFileSize,intFindType,intDebug,intUC,intEndian,intGAP,intNoNulls,intMinLen,intNonPrintable);
	if(intRet>0){ // return an error
		return intRet;
	}

	//
	
	return 0;
}

// -------------------------------------------------------------------------
// Function: Entry point
// -------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	// Vars
	int				intInFile=0,intFindType=1, intUC=0, intDebug=0, intEndian=1,intNoNulls=0, intNonPrintable=0,intFindCnt=0,intEndianCnt=0;
	unsigned int	intMinLen=1,intGAP=0;
	char			*strInFile;
	char			chOpt;
	
	// Print banner
	banner();
	 
	// Extract all the options
	while ((chOpt = getopt(argc, argv, "i:g:m:p:ABWDudhsn")) != -1) 
	switch(chOpt)
	{
		case 'i': // input file
			intInFile=1;
			strInFile=optarg;
			break;
		case 'u': // unicode
			intUC=1;
			break;
		case 'm': // min len
			intMinLen=atoi(optarg);
			break;
		case 'g': // gap
			intGAP=atoi(optarg);
			break;
		case 'd': // debugging
			intDebug=1;
			break;
		case 'n': // no nulls
			intNoNulls=1;
			break;
		case 'p': // non printable
			intNonPrintable=atoi(optarg);
			break;
		case 'B': // byte mode
			intFindType=1;
			break;
		case 'W': // word mode
			intFindType=2;
			break;
		case 'D': // dword mode
			intFindType=3;
			break;
		case 'A': // all of em
			intFindType=4;
			break;
		case 's': // endian
			intEndian=2;
			break;
		case 'h': // 192
			bannerhelp(argv[0]);
			goto exit;
			break;
		default:
			bannerhelp(argv[0]);
			goto exit;
   	}

	// Error checking
	if(!intInFile){
		bannerhelp(argv[0]);
		goto exit;
	}

	if(intDebug)			fprintf(stdout,"[*] Debugging on!\n");
	if(intGAP>0)			fprintf(stdout,"[*] Gap beween LEN and DATA %d\n",intGAP);
	if(intMinLen>1)			fprintf(stdout,"[*] Minimum length %d\n",intMinLen);
	if(intUC==1)			fprintf(stdout,"[*] UNICODE mode on!\n");
	if(intNoNulls==1)		fprintf(stdout,"[*] NoNulls mode on!\n");
	if(intFindType==4)		fprintf(stdout,"[*] BYTE, WORD and DWORD modes on!\n");

	if(intNonPrintable>0){
		if (intNonPrintable==255){
			fprintf(stdout,"[*] Nonprintable chars allowed in string: Unlimited\n",intNonPrintable);
		} else{
			fprintf(stdout,"[*] Nonprintable chars allowed in string: %d\n",intNonPrintable);
		}
	}

	// Do the magic
	if (intFindType < 4){ // Standard find type
		switch(FindLensWrapper(strInFile,intFindType,intDebug,intUC,intEndian,intGAP,intNoNulls,intMinLen,intNonPrintable)){
			case 0: // no errors
				fprintf(stdout,"[*] Completed successfully!\n");
				break;
			case 1: // couldn't open file
				fprintf(stdout,"[!] Couldn't open source file!\n");
				break;
			case 5:	// malloc failed	
				fprintf(stdout,"[!] Memory allocation error!\n");
				break;
			case 6:	// malloc failed	
				fprintf(stdout,"[!] Couldn't read all of file!\n");
				break;
			case 99: // unknown error
				fprintf(stdout,"[!] Unknown error!\n");
				break;
			default:
				fprintf(stdout,"[!] Really unknown error!\n");
				break;
		}
	} else if (intFindType==4) { // All find types
		for(intEndianCnt=1;intEndianCnt<=intEndian;intEndianCnt++){ // if -S
			for(intFindCnt=1;intFindCnt<4;intFindCnt++){
				switch(FindLensWrapper(strInFile,intFindCnt,intDebug,intUC,intEndianCnt,intGAP,intNoNulls,intMinLen,intNonPrintable)){
					case 0: // no errors
						fprintf(stdout,"[*] Completed successfully!\n");
						break;
					case 1: // couldn't open file
						fprintf(stdout,"[!] Couldn't open source file!\n");
						break;
					case 5:	// malloc failed	
						fprintf(stdout,"[!] Memory allocation error!\n");
						break;
					case 6:	// malloc failed	
						fprintf(stdout,"[!] Couldn't read all of file!\n");
						break;
					case 99: // unknown error
						fprintf(stdout,"[!] Unknown error!\n");
						break;
					default:
						fprintf(stdout,"[!] Really unknown error!\n");
						break;
				}
			}
		}
	}

	// Shutdown
exit:	
    bannerstop();
	return 0;
}

