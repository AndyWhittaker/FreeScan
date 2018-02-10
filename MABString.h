#if !defined(AFX_STRINGDEG_H__3A310B45_A6C3_11D1_B56D_00A024838B6B__INCLUDED_)
#define AFX_STRINGDEG_H__3A310B45_A6C3_11D1_B56D_00A024838B6B__INCLUDED_
// COPYRITE 1995 MAB TECH , 96,97
// Base code is derivitive of Zafir's CString class found on the www.CodeGuru.com site.
class CStringEx : public CString
{
public:
	void Accuracy(int acc,int ExpAscii);
	// Code From
	// Copyright SOCABIM 1999
	// Pierre Caussin
	// Jean-Paul Retru
	static CString m_csNAN;
	static CString m_csNINF;
	static CString m_csPINF;
	//
	void TestExp();
	inline double atodn(char alpha[],int n);
	inline float atofn(char *alpha,int n);
	inline int atoin(char *alpha,int n);
	inline long int atoln(char alpha[],int n);
	inline void dtoan(char *alpha, double val, int len);
//	inline void ftoan(char *alpha,float val,int len);
	inline void itoan(char alpha[],int n,int l);
	inline BOOL ltoan(char alpha[],long int n,int l);
//	inline int sizestr(char string[],int max);
//	inline void stradd(char stringto[],int maxto,char stringfrom[],int maxfrom);
//	inline void strfill(char string[],int max,char fill);
//	inline void strover(char stringto[],char stringfrom[],int maxfrom);
	int GetInt();
	long GetLong();
	float GetFloat();
	double GetDouble();
	void PutLong(long i, int len=8);
	void PutInt(int i, int len=8);
	void PutFloat(float value, int len=8, char ExpChar=69);
	void PutDouble(double value, int len=8, char ExpChar=69);
	void PutFloatS(float value, int nsignificands=8, char ExpChar=69);
	void PutDoubleS(double value, int nsignificands=8, char ExpChar=69);
	BOOL IsInt();
	BOOL IsFloat();
	CStringEx() : CString( ){};
	CStringEx( const CString& stringSrc) : CString( stringSrc ){};
	CStringEx( const CStringEx& stringSrc) : CString( stringSrc ){};
	CStringEx( TCHAR ch, int nRepeat = 1 ) : CString( ch, nRepeat ){};
	CStringEx( LPCTSTR lpch, int nLength ) : CString( lpch, nLength ){};
	CStringEx( const unsigned char* psz ) : CString( psz ){};
	CStringEx( LPCWSTR lpsz ) : CString( lpsz ){};
	CStringEx( LPCSTR lpsz ) : CString( lpsz ){};

	CStringEx& Insert(int pos, LPCTSTR s);
	CStringEx& Insert(int pos, TCHAR c);

	CStringEx& Delete(int pos, int len);
	CStringEx& Replace(int pos, int len, LPCTSTR s);

	int Find( TCHAR ch, int startpos = 0 ) const;
	int Find( LPCTSTR lpszSub, int startpos = 0 ) const;
	int FindNoCase( TCHAR ch, int startpos = 0 ) const;
	int FindNoCase( LPCTSTR lpszSub, int startpos = 0 ) const;

	int FindReplace( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith, BOOL bGlobal = TRUE );
	int FindReplaceNoCase( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith,
				BOOL bGlobal = TRUE );

	int ReverseFind( TCHAR ch ) const{ return CString::ReverseFind(ch);};
	int ReverseFind( LPCTSTR lpszSub, int startpos = -1 ) const;
	int ReverseFindNoCase( TCHAR ch, int startpos = -1  ) const;
	int ReverseFindNoCase( LPCTSTR lpszSub, int startpos = -1 ) const;

	CStringEx GetField( LPCTSTR delim, int fieldnum);
	CStringEx GetField( TCHAR delim, int fieldnum);
	int GetFieldCount( LPCTSTR delim );
	int GetFieldCount( TCHAR delim );

	CStringEx GetDelimitedField( LPCTSTR delimStart, LPCTSTR delimEnd,
				int fieldnum = 0);
};

#endif // !defined(AFX_STRINGDEG_H__3A310B45_A6C3_11D1_B56D_00A024838B6B__INCLUDED_)
