// Base code is derivitive of Zafir's CString class found on the www.CodeGuru.com site.

/* For future reference from a post from 2008 in codeguru:
Zapper, that article is 9 YEARS OLD.

The missing features referred to by the article to have long been implemented in CString.

In addition, back then CString had a dependency on MFC, but since Visual C++ 2005,
his dependency has been removed and MFC is no longer required to use CString. The underlying
CString functionality is implemented by CStringT and you just need to include "atlstr.h" to use it in your app.

You can read all about the CStringT class in msdn.
P.S. Don't use CStringT directly, just include "atlstr.h" and
use CString (including the header will properly typedef CStringT into CString for you).
*/

#include "StdAfx.h"
#include "MABString.h"
#include "math.h"
#include "float.h"

// Insert	- Inserts a sub string into the string
// Returns	- Reference to the same string object
// pos		- Position to insert at. Extends the string with spaces if needed
// s		- Sub string to insert
CStringEx& CStringEx::Insert(int pos, LPCTSTR s)
{
	int len = lstrlen(s);
	if ( len == 0 )
		return *this;

	int oldlen = GetLength();
	int newlen = oldlen + len;
	LPTSTR str;
	if ( pos >= oldlen )
	{			
		// insert after end of string
		newlen += pos - oldlen ;
		str = GetBuffer( newlen );
		_tcsnset_s( str+oldlen,newlen, _T(' '), pos-oldlen );
		_tcsncpy_s( str+pos,newlen, s, len );
	}
	else
	{	
		// normal insert
		str = GetBuffer( newlen );
		memmove( str+pos+len, str+pos, sizeof(_T(' ')) *(oldlen-pos) );
		_tcsncpy_s( str+pos, newlen, s, len );
	}
	ReleaseBuffer( newlen );

	return *this;
}


// Insert	- Inserts a character into the string
// Returns	- Reference to the same string object
// pos		- Position to insert at. Extends the string with spaces if needed
// c		- Char to insert
CStringEx& CStringEx::Insert(int pos, TCHAR c)
{
	TCHAR buf[2];
	buf[0] = c;
	buf[1] = _T('\0');
	return Insert( pos, buf );
}


// Delete	- Deletes a segment of the string and resizes it
// Returns	- Reference to the same string object
// pos		- Position of the string segment to remove
// len		- Number of characters to remove
CStringEx& CStringEx::Delete(int pos, int len)
{
	int strLen = GetLength();

	if( pos >= strLen)
		return *this;
	if(len < 0 ||len > strLen - pos)
		len = strLen - pos;

	LPTSTR str = GetBuffer( strLen );
	memmove(str+pos, str+pos+len, sizeof(_T(' ')) *(strLen-pos));
	ReleaseBuffer( strLen - len );

	return *this;
}


// Replace	- Replaces a substring with another
// Returns	- Reference to the same string object
// pos		- Position of the substring
// len		- Length of substring to be replaced
// s		- New substring
CStringEx& CStringEx::Replace(int pos, int len, LPCTSTR s)
{
	Delete(pos, len);
	Insert(pos, s);

	return *this;
}


// Find 	- Finds the position of a character in a string
// Returns	- A zero-based index
// ch		- Character to look for
// startpos	- Position to start looking from
int CStringEx::Find( TCHAR ch, int startpos /*= 0*/ ) const
{
	// find first single character
	LPTSTR lpsz = _tcschr((LPTSTR)GetString() + startpos, (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - GetString());
}


// Find 	- Finds the position of a substring in a string
// Returns	- A zero-based index
// lpszSub	- Substring to look for
// startpos	- Position to start looking from
int CStringEx::Find( LPCTSTR lpszSub, int startpos /*= 0*/ ) const
{
	ASSERT(AfxIsValidString(lpszSub, FALSE));

	// find first matching substring
	LPTSTR lpsz = _tcsstr((LPTSTR)GetString()+startpos, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - GetString());
}


// FindNoCase	- Case insensitive find
// Returns	- A zero-based index
// ch		- Char to search for
// startpos 	- Position to start looking from
int CStringEx::FindNoCase( TCHAR ch, int startpos /*= 0*/ ) const
{
	unsigned int locase = Find( tolower( ch ), startpos );
	unsigned int upcase = Find( toupper( ch ), startpos );

	return locase < upcase ? locase : upcase;
}


// FindNoCase	- Case insensitive find
// Returns	- A zero-based index
// lpszSub	- Substring to search for
// startpos 	- Position to start looking from
int CStringEx::FindNoCase( LPCTSTR lpszSub, int startpos /*= 0*/ ) const
{
	CStringEx sLowerThis = *this;
	sLowerThis.MakeLower();

	CStringEx sLowerSub = lpszSub;
	sLowerSub.MakeLower();

	return sLowerThis.Find( sLowerSub, startpos );
}


// FindReplace		- Find a substring and replace with another
// Returns		- Number of instances replaced
// lpszSub		- Substring to look for
// lpszReplaceWith	- Substring to replace with
// bGlobal		- Flag to indicate whether all occurances
//					  should be replaced
int CStringEx::FindReplace( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith,
					BOOL bGlobal /*= TRUE*/ )
{
	int iReplaced = 0;

	// find first matching substring
	LPTSTR lpsz;
	
	int pos = 0;
	int lenSub = lstrlen( lpszSub );
	int lenReplaceWith = lstrlen( lpszReplaceWith );
	while( (lpsz = _tcsstr((LPTSTR)GetString() + pos, lpszSub)) != NULL )
	{
		pos = (int)(lpsz - (LPTSTR)GetString());
		Replace( pos, lenSub, lpszReplaceWith );
		pos += lenReplaceWith;
		iReplaced++;
		if( !bGlobal ) break;
	}

	return iReplaced;
}


// FindReplaceNoCase	- Find a substring and replace with another
//			  Does case insensitive search
// Returns		- Number of instances replaced
// lpszSub		- Substring to look for
// lpszReplaceWith	- Substring to replace with
// bGlobal		- Flag to indicate whether all occurances
//			  should be replaced
int CStringEx::FindReplaceNoCase( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith, 
					 BOOL bGlobal /*= TRUE*/ )
{
	CStringEx sLowerThis = *this;
	sLowerThis.MakeLower();

	CStringEx sLowerSub = lpszSub;
	sLowerSub.MakeLower();

	int iReplaced = 0;

	// find first matching substring
	LPTSTR lpsz;
	
	int pos = 0, offset = 0;
	int lenSub = lstrlen( lpszSub );
	int lenReplaceWith = lstrlen( lpszReplaceWith );
	while( (lpsz = _tcsstr((LPTSTR)sLowerThis.GetString() + pos, (LPTSTR)sLowerSub.GetString())) 
!= NULL )
	{
		pos = (int)(lpsz - (LPTSTR)sLowerThis.GetString());
		Replace( pos+offset, lenSub, lpszReplaceWith );
		offset += lenReplaceWith - lenSub;
		pos += lenSub;
		iReplaced++;
		if( !bGlobal ) break;
	}

	return iReplaced;
}


// ReverseFind	- Searches for the last match of a substring
// Returns	- A zero-based index
// lpszSub	- Substring to search for
// startpos 	- Position to start looking from, in reverse dir
int CStringEx::ReverseFind( LPCTSTR lpszSub, int startpos /*= -1*/ ) const
{
	int lenSub = lstrlen( (LPTSTR)GetString() );
	int len = lstrlen( (LPTSTR)GetString() );

	if(0 < lenSub && 0 < len)
	{
		if( startpos == -1 || startpos >= len ) startpos = len - 1;
		for ( LPTSTR lpszReverse = (LPTSTR)GetString() + startpos ;
						lpszReverse != (LPTSTR)GetString() ; --lpszReverse)
			if (_tcsncmp(lpszSub, lpszReverse, lenSub ) == 0)
				return (lpszReverse - (LPTSTR)GetString());
	}
	return -1;
}


// ReverseFindNoCase	- Searches for the last match of a substring
//			  Search is case insensitive
// Returns		- A zero-based index
// lpszSub		- Character to search for
// startpos 		- Position to start looking from, in reverse dir
int CStringEx::ReverseFindNoCase(TCHAR ch, int startpos /*= -1*/ ) const
{
	TCHAR a[3];
	a[1] = ch;
	a[2] = 0;
	return ReverseFindNoCase( a, startpos );
}


// ReverseFindNoCase	- Searches for the last match of a substring
//			  Search is case insensitive
// Returns		- A zero-based index
// lpszSub		- Substring to search for
// startpos 		- Position to start looking from, in reverse dir
int CStringEx::ReverseFindNoCase( LPCTSTR lpszSub, int startpos /*= -1*/ ) 
const
{
	//LPTSTR lpszEnd = m_pchData + lstrlen

	int lenSub = lstrlen( lpszSub );
	int len = lstrlen( (LPTSTR)GetString() );
	

	if(0 < lenSub && 0 < len)
	{
		if( startpos == -1 || startpos >= len ) startpos = len - 1;
		for ( LPTSTR lpszReverse = (LPTSTR)GetString() + startpos ;
				lpszReverse >= (LPTSTR)GetString() ; --lpszReverse)
			if (_tcsnicmp(lpszSub, lpszReverse, lenSub ) == 0)
				return (lpszReverse - (LPTSTR)GetString());
	}
	return -1;
}


// GetField 	- Gets a delimited field
// Returns	- A CStringEx object that contains a copy of
//		  the specified field
// delim	- The delimiter string
// fieldnum 	- The field index - zero is the first
// NOTE 	- Returns the whole string for field zero
//		  if delim not found
//		  Returns empty string if # of delim found
//		  is less than fieldnum
CStringEx CStringEx::GetField( LPCTSTR delim, int fieldnum)
{
	LPTSTR lpsz, lpszRemainder = (LPTSTR)GetString(), lpszret;
	int retlen, lenDelim = lstrlen( delim );

	while( fieldnum-- >= 0 )
	{
		lpszret = lpszRemainder;
		lpsz = _tcsstr(lpszRemainder, delim);
		if( lpsz )
		{
			// We did find the delim
			retlen = lpsz - lpszRemainder;
			lpszRemainder = lpsz + lenDelim;
		}
		else
		{
			retlen = lstrlen( lpszRemainder );
			lpszRemainder += retlen;	// change to empty string
		}
	}
	return Mid( lpszret - (LPTSTR)GetString(), retlen );
}

// GetField 	- Gets a delimited field
// Returns	- A CStringEx object that contains a copy of
//		  the specified field
// delim	- The delimiter char
// fieldnum 	- The field index - zero is the first
// NOTE 	- Returns the whole string for field zero
//		  if delim not found
//		  Returns empty string if # of delim found
//		  is less than fieldnum
CStringEx CStringEx::GetField( TCHAR delim, int fieldnum)
{
	LPTSTR lpsz, lpszRemainder = (LPTSTR)GetString(), lpszret;
	int retlen;

	while( fieldnum-- >= 0 )
	{
		lpszret = lpszRemainder;
		lpsz = _tcschr(lpszRemainder, (_TUCHAR)delim);
		if( lpsz )
		{
			// We did find the delim
			retlen = lpsz - lpszRemainder;
			lpszRemainder = lpsz + 1;
		}
		else
		{
			retlen = lstrlen( lpszRemainder );
			lpszRemainder += retlen;	// change to empty string
		}
	}
	return Mid( lpszret - (LPTSTR)GetString(), retlen );
}


// GetFieldCount	- Get number of fields in a string
// Returns		- The number of fields
//			  Is always greater than zero
// delim		- The delimiter character
int CStringEx::GetFieldCount( TCHAR delim )
{
	TCHAR a[2];
	a[0] = delim;
	a[1] = 0;
	return GetFieldCount( a );
}


// GetFieldCount	- Get number of fields in a string
// Returns		- The number of fields
//			  Is always greater than zero
// delim		- The delimiter string
int CStringEx::GetFieldCount( LPCTSTR delim )
{
	LPTSTR lpsz, lpszRemainder = (LPTSTR)GetString();
	int lenDelim = lstrlen( delim );

	int iCount = 1;
	while( (lpsz = _tcsstr(lpszRemainder, delim)) != NULL )
	{
		lpszRemainder = lpsz + lenDelim;
		iCount++;
	}
	return iCount;
}


// GetDelimitedField	- Finds a field delimited on both ends
// Returns		- A CStringEx object that contains a copy of
//			  the specified field
// delimStart		- Delimiter at the start of the field
// delimEnd 		- Delimiter at the end of the field
CStringEx CStringEx::GetDelimitedField( LPCTSTR delimStart, LPCTSTR 
delimEnd, int fieldnum /*= 0*/)
{
	LPTSTR lpsz, lpszEnd, lpszRet, lpszRemainder = (LPTSTR)GetString() ;
	int lenDelimStart = lstrlen( delimStart ), lenDelimEnd = lstrlen( delimEnd 
);

	while( fieldnum-- >= 0 )
	{
		lpsz = _tcsstr(lpszRemainder, delimStart);
		if( lpsz )
		{
			// We did find the Start delim
			lpszRet = lpszRemainder = lpsz + lenDelimStart;
			lpszEnd = _tcsstr(lpszRemainder, delimEnd);
			if( lpszEnd == NULL ) return"";
			lpszRemainder = lpsz + lenDelimEnd;
		}
		else return "";
	}
	return Mid( lpszRet - (LPTSTR)GetString(), lpszEnd - lpszRet );
}

BOOL CStringEx::IsFloat()
{
	if(!Find(".")) return FALSE;
	return TRUE;
}

BOOL CStringEx::IsInt()
{
	if(Find(".")) return FALSE;
	return TRUE;
}

void CStringEx::PutFloat(float value, int len, char ExpChar)
{
	switch	(_fpclass(value))
	{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
			Format("%s", m_csNAN); // "NaN"
			return;
		case _FPCLASS_NINF:
			Format("%s", m_csNINF);// "-Inf"
			return;
		case _FPCLASS_PINF:
			Format("%s", m_csPINF);// "+Inf"
			return;
	}
	PutDouble((double)value, len, ExpChar);
	// have we exceeded our acuracy?
	int acc = 8;
	if(len< acc+1) return;
	Accuracy(acc,(int)ExpChar);
}

void CStringEx::Accuracy(int acc,int ExpAscii)
{
	int len = GetLength();
	int i=0,i1;
	if(GetAt(i)==35) return;
	while(GetAt(i)==48 || GetAt(i)==45 || GetAt(i)==46)
	{
		if (GetAt(i)==ExpAscii) return;
		i++;
		if (i>=len) break;
	}
	i1=i-1;
	if(acc+i1+1>len) i1=len-acc-1;
	while(i<=acc+i1)
	{
		if(GetAt(i)==ExpAscii) return;
		i++;
	}
	if(Find('.') > i-acc && Find('.') < i) i++;
	for(;i<len;i++)
	{
		if(GetAt(i)==ExpAscii) return;
		if(GetAt(i)!=46) SetAt(i,'0');
	}
}

void CStringEx::PutFloatS(float value, int nsignificands, char ExpChar)
{
	// Code From
	// Copyright SOCABIM 1999
	// Pierre Caussin
	// Jean-Paul Retru
	switch	(_fpclass(value))
	{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
			Format("%s", m_csNAN); // "NaN"
			return;
		case _FPCLASS_NINF:
			Format("%s", m_csNINF);// "-Inf"
			return;
		case _FPCLASS_PINF:
			Format("%s", m_csPINF);// "+Inf"
			return;
	}
	PutDoubleS((double) value, nsignificands, ExpChar);
	int acc = 8;
	if(nsignificands< acc+1) return;
	Accuracy(acc,(int)ExpChar);
	return;
}

void CStringEx::PutDoubleS(double value, int nsignificands, char ExpChar)
{
	Empty( );
	// Code From
	// Copyright SOCABIM 1999
	// Pierre Caussin
	// Jean-Paul Retru
	CString fmt;
	int nk;
	switch	(_fpclass(value))
		{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
			Format("%s", m_csNAN); // "NaN"
			return;
		case _FPCLASS_NINF:
			Format("%s", m_csNINF);// "-Inf"
			return;
		case _FPCLASS_PINF:
			Format("%s", m_csPINF);// "+Inf"
			return;
		case _FPCLASS_NN:
		case _FPCLASS_PN:
			nk = (int)log10(fabs(value));
			if	(fabs(value)<1) nk--;
			if	(nk > nsignificands-1 || nk < -2)
				{
				fmt.Format("%%.%df%c%d",nsignificands-1,ExpChar,nk);
				Format(fmt,value/pow(10.,nk));
				}
			else
				{
				if	(nk == nsignificands-1 && nk>1)
					fmt.Format("%%.%df.",0);
				else
					fmt.Format("%%.%df",nsignificands>nk+1?nsignificands-nk-1:0);
				Format(fmt,value);
				}
			return;
		default:
			fmt.Format("%%.%df",nsignificands);
			Format(fmt,0.);
	}
	int acc = 16;
	Accuracy(acc,(int)ExpChar);
	return;
}

void CStringEx::PutDouble(double value, int len, char ExpChar)
{
	Empty( );
	switch	(_fpclass(value))
	{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
			Format("%s", m_csNAN); // "NaN"
			return;
		case _FPCLASS_NINF:
			Format("%s", m_csNINF);// "-Inf"
			return;
		case _FPCLASS_PINF:
			Format("%s", m_csPINF);// "+Inf"
			return;
	}
	
	dtoan(GetBuffer(len+1),value,len);
	ReleaseBuffer(len+1);
	// have we exceeded our acuracy?
	// This line replaces the "E" with the users exponent definition.
	if(Find('E')>0 && ExpChar!=69) SetAt(Find('E')-1,ExpChar);
	int acc = 16;
	if(len< acc+1) return;
	Accuracy(acc,(int)ExpChar);
}

void CStringEx::PutLong(long i, int len)
{
	Empty( );
	if(i>=0)
	{
		if(ltoan(GetBuffer(len),i,len))
			ReleaseBuffer(len);
		else
		{
			ReleaseBuffer(len);
			Format("Error");
		}
	}
	else
	{
		ltoan(GetBuffer(len),abs(i),len);
		ReleaseBuffer(len);
		i=0;
		while(GetAt(i)==32) i++;
		if(i>0) SetAt(i-1,'-');
		else Format("Error");
	}
}

void CStringEx::PutInt(int i, int len)
{
	PutLong((long)i,len);
}

float CStringEx::GetFloat()
{
	TestExp();
	float f;
	f = atofn(GetBuffer(GetLength()),GetLength());
	return f;
}

double CStringEx::GetDouble()
{
	TestExp();
	double f;
	f = atodn(GetBuffer(GetLength()),GetLength());
	return f;
}

long CStringEx::GetLong()
{
	long f;
	f = atoln(GetBuffer(GetLength()),GetLength());
	return f;
}

int CStringEx::GetInt()
{
	return atoin(GetBuffer(GetLength()),GetLength());
}

inline double CStringEx::atodn(char alpha[],int n)
{
 double val;
 char format[16];
 char string[32];
 sprintf_s(format,"%c%df",'%',n);
 if(n>32) n=32;
 strncpy_s(string,alpha,n);
 string[n]='\0';
 val=atof(string);//0.0;
 //sscanf(string,format,&val);
 return (val);
}

inline float CStringEx::atofn(char *alpha,int n)
{
 float val;
 char format[16];
 char string[32];
 sprintf_s(format,"%c%df",'%',n);
 if(n>32) n=32;
 strncpy_s(string,alpha,n);
 string[n]='\0';
 val=(float)0.;
 sscanf_s(string,format,&val);
 return (val);
}

inline int CStringEx::atoin(char *alpha,int n)
{
 int val;
 char format[32];
 char string[32];
 sprintf_s(format,"%c%dd",'%',n);
 val=0;
 if(n>32)n=32;
 strncpy_s(string,alpha,n);
 string[n]='\0';
 sscanf_s(string,format,&val);
 return (val);
}

inline long int CStringEx::atoln(char alpha[],int n)
{
 long int val;
 char string[32];
 if(n>31) return(0);
 strncpy_s(string,alpha,n);
 string[n]='\0';
 val=atol(string);
 return (val);
}

inline void CStringEx::dtoan(char *alpha,double val,int len)
{
	if(len<4)
	{
		AfxMessageBox("Minimum String Length is 4 for error handling!");
		return;
	}
 char format[16];
 int exp,i;
 unsigned int lenx;
 char *temp;
 // do we need to use exponential 
 if(val < 0.0) sprintf_s(format,"%9.2E",val);
 else          sprintf_s(format,"%9.2E",val);
 exp=strlen(&format[6]);
 exp=abs(atoin(&format[6],exp));
 if( (val>=0 && exp+3 >len) || (val<0 && exp+4 >len) || (exp>2 && fabs(val)<1.0))
 { // use E format 
	lenx=len+11;
	i=sizeof(char);
	temp=(char near *)calloc(lenx,i);
	if(val < 0.0) sprintf_s(format,"%c%d.%dE",'%',len-1,len-5);
	else          sprintf_s(format,"%c%d.%dE",'%',len,len-4);
	sprintf_s(temp,sizeof(temp),format,val);
	lenx=strlen(temp);
	i=lenx-3;
	do {
      if(temp[i]=='0') strcpy_s(&temp[i],sizeof(temp),&temp[i+1]);
      } while(i<(int)strlen(temp) && temp[i]=='0');
	do	{
		i--;
		} while(i>0 && temp[i]!='E');
	if(temp[i]=='E' && temp[i+1]=='+') strcpy_s(&temp[i+1],sizeof(temp),&temp[i+2]);
	while (len < (int) strlen(temp) && i>3)
      {
      i--;
      strcpy_s(&temp[i],sizeof(temp),&temp[i+1]);
      }
	if((int)strlen(temp)>len) strcpy_s(alpha,sizeof(temp),"ERR");
	else strcpy_s(alpha,sizeof(temp),temp);
   free(temp);
 }
 else
 { // use f format 
   if(val < 0.0f) sprintf_s(format,16, "%c%d.%df",'%',len-1,len-3-exp);
   else if (val< 0.) sprintf_s(format,16, "%c%d.%df",'%',len,len-3);
   else if (val< 1.) sprintf_s(format,16, "%c%d.%df",'%',len,len-2);
   else          sprintf_s(format,16, "%c%d.%df",'%',len,len-2-exp);
   sprintf_s(alpha,sizeof(alpha),format,val);
 }
 return;
}


/*
inline void CStringEx::ftoan(char *alpha,float val,int len)
{
 char format[16];
 int exp,i;
 unsigned int lenx;
 char *temp;
 // do we need to use exponential
 if(val < 0.0) sprintf_s(format,"%7.2E",val);
 else          sprintf_s(format,"%8.2E",val);
 exp=strlen(&format[6]);
 exp=atoin(&format[6],exp);
 if( (val>=0 && exp+3 >len) || (val<0 && exp+4 >len) || (exp>2 && fabs(val)<1.0))
 { // use E format
   lenx=len+7;
   i=sizeof(char);
   temp=(char near *)calloc(lenx,i);
   if(val < 0.0) sprintf_s(format,"%c%d.%dE",'%',len-1,len-5);
   else          sprintf_s(format,"%c%d.%dE",'%',len,len-4);
   sprintf_s(temp,format,val);
   lenx=strlen(temp);
   i=lenx-2;
   do {
      if(temp[i]=='0') strcpy(&temp[i],&temp[i+1]);
      else i--;
      } while(i>0 && temp[i]!='E');
	if(temp[i]=='E' && temp[i+1]=='+') strcpy(&temp[i+1],&temp[i+2]);
	while (len < (int) strlen(temp))
      {
      i--;
      strcpy(&temp[i],&temp[i+1]);
      }
   strcpy(alpha,temp);
   free(temp);
 }
 else
 { // use f format
   if(val < -1.0) sprintf_s(format,"%c%d.%df",'%',len-1,len-3-exp);
   else if (val< 0.) sprintf_s(format,"%c%d.%df",'%',len-1,len-3);
   else if (val< 1.) sprintf_s(format,"%c%d.%df",'%',len,len-2);
   else          sprintf_s(format,"%c%d.%df",'%',len,len-2-exp);
   sprintf_s(alpha,format,val);
 }
 return;
}
*/

inline void CStringEx::itoan(char alpha[],int n,int l)
{
 int val;
 int count;
 char item;
 for (count=1;count<=l;count++)
 {
 val=n-(n/10)*10;
 _itoa_s(val,&item,sizeof(item),10);
 alpha[l-count]=item;
 n=n/10;
 }
 count=0;
 while ((int)alpha[count]==48 && count<l-1)
 {
	alpha[count]=' ';
	count++;
 }

 return;
}

inline BOOL CStringEx::ltoan(char alpha[],long int n,int l)
{
 long int val;
 int count;
 char item;
 BOOL ret=FALSE;
 for (count=1;count<=l;count++)
 {
 val=n-(n/10)*10;
 _ltoa_s(val,&item,sizeof(item),10);
 alpha[l-count]=item;
 n=n/10;
 }
 if(n==0) ret=TRUE;
 count=0;
 while ((int)alpha[count]==48 && count<l-1)
 {
	alpha[count]=' ';
	count++;
 }
 return ret;
}

/*
inline int CStringEx::sizestr(char string[],int max)
{
  int i;
  for(i=0;i<max;i++)
  {
  if(string[i]=='\0') return(i);
  }
  return(max);
}

inline void CStringEx::stradd(char stringto[],int maxto,char stringfrom[],int maxfrom)
{
 int l1,l2,i;
 l1=sizestr(stringto,maxto);
 l2=sizestr(stringfrom,maxfrom);
 for(i=0;i<=l2;i++) stringto[l1+i]=stringfrom[i];
 return;
}

inline void CStringEx::strfill(char string[],int max,char fill)
// max is length of string including null terminator
{
 int l1,i;
 string[max-1]='\0';
 l1 = sizestr(string,max);
// /* l1 is length of the string 
//    this matchs the position
//    of the null terminator
 for(i=l1;i<=max-2;i++) string[i]=fill;
 string[max-1]='\0';
 return;
}

inline void CStringEx::strover(char stringto[],char stringfrom[],int maxfrom)
{
 int l2,i;
 l2=sizestr(stringfrom,maxfrom)-1;
 for(i=0;i<=l2;i++) stringto[i]=stringfrom[i];
 return;
}
*/

void CStringEx::TestExp()
{
 int pos;
 pos = max(ReverseFind('-'),ReverseFind('+'));
 if(pos<2) return;
 if(GetAt(pos-1)==32) return;
 if(GetAt(pos-1)==69 || GetAt(pos-1)==101) return;
 Insert(pos-1,'E');
}


// The following code from
// Copyright SOCABIM 1999
// Pierre Caussin
// Jean-Paul Retru

CString CStringEx::m_csNAN("NaN");
CString CStringEx::m_csNINF("-Inf");
CString CStringEx::m_csPINF("+Inf");

