// Msg.h: interface for the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__01F20851_C393_40B0_A45A_DB48FA4CA3FA__INCLUDED_)
#define AFX_MSG_H__01F20851_C393_40B0_A45A_DB48FA4CA3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSG_DEFAULT_SIZE	4096
#define MSG_HEADER_SIZE		4
#define MSG_INVALID_MSG		65535

class CMsg  
{
protected:
	BYTE	m_buf[MSG_DEFAULT_SIZE];
	LPBYTE	m_pHead;	
	LPBYTE	m_pData;	
	WORD	m_nRdOff;
	WORD	m_nWrOff;

	void ReadData (const LPVOID pData, int n);
	void WriteData (LPVOID pData, int n);

public:
	CMsg();
	CMsg(CMsg& msg);
	CMsg(LPVOID pBuf);

	CMsg& ID( WORD id );
	WORD  ID();

	LPVOID GetBuf ();
	void SetBuf (LPVOID pBuf);
	WORD GetSize ();
	BOOL IsReadAll ();
	void Copy (LPVOID pBuf);
	void Clear ();
	void Decrypt(BYTE bKey);
	void Encrypt(BYTE bKey);

// Archive Operators
	CMsg&		operator<<( char		arg );
	CMsg&		operator<<( short		arg );
	CMsg&		operator<<( int		arg );
	CMsg&		operator<<( BYTE		arg );
	CMsg&		operator<<( WORD		arg );
	CMsg&		operator<<( DWORD		arg );
	CMsg&		operator<<( INT64		arg );
	CMsg&		operator<<( LPCTSTR	arg );
	CMsg&		operator<<( CMsg* arg );

	CMsg&		operator>>( char&		arg );	
	CMsg&		operator>>( short&		arg );	
	CMsg&		operator>>( int&		arg );	
	CMsg&		operator>>( BYTE&		arg );
	CMsg&		operator>>( WORD&		arg );	
	CMsg&		operator>>( DWORD&		arg );
	CMsg&		operator>>( INT64&		arg );	
//	CMsg&		operator>>( CString&	arg );	
	CMsg&		operator>>( sString&	arg );

	CMsg&		operator=( CMsg&		msg );	
};

#endif // !defined(AFX_MSG_H__01F20851_C393_40B0_A45A_DB48FA4CA3FA__INCLUDED_)


