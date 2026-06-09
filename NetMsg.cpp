// Msg.cpp: implementation of the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "precompile.h"
#include "NetMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsg::CMsg()
{
	m_pHead = m_buf;
	m_pData = m_pHead + MSG_HEADER_SIZE;
	Clear ();
}

CMsg::CMsg(CMsg& msg)
{
	m_pHead = m_buf;
	m_pData = m_pHead + MSG_HEADER_SIZE;
	m_nRdOff = 0;
	m_nWrOff = msg.m_nWrOff;
	CopyMemory (m_pHead, msg.m_pHead, MSG_DEFAULT_SIZE);
}

CMsg::CMsg(LPVOID pBuf)
{
	m_pHead = m_buf;
	m_pData = m_pHead + MSG_HEADER_SIZE;
	m_nRdOff = 0;
	m_nWrOff = *((LPWORD)((LPBYTE)pBuf + 2));
	CopyMemory (m_pHead, pBuf, m_nWrOff + MSG_HEADER_SIZE);
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CMsg::ReadData (const LPVOID pData, int n)
{
	if (m_nRdOff + n + MSG_HEADER_SIZE > MSG_DEFAULT_SIZE)
		return;

	CopyMemory (pData, m_pData + m_nRdOff, n);
	m_nRdOff += n;
}

void CMsg::WriteData (LPVOID pData, int n)
{
	if (m_nWrOff + n + MSG_HEADER_SIZE > MSG_DEFAULT_SIZE)
		return;

	CopyMemory (m_pData + m_nWrOff, pData, n);
	m_nWrOff += n;
	CopyMemory (m_pHead + 2, &m_nWrOff, 2);
}

void CMsg::Clear ()
{
	m_nRdOff = 0; 
	m_nWrOff = 0;
	ZeroMemory (m_pHead, MSG_HEADER_SIZE);
}

CMsg& CMsg::ID( WORD id )
{
	Clear ();
	*((LPWORD)(m_pHead)) = id;

	return *this;
}

WORD CMsg::ID()
{
	return *((LPWORD)m_pHead);
}

LPVOID CMsg::GetBuf ()
{
	return (LPVOID)m_pHead;
}

void CMsg::SetBuf (LPVOID pBuf)
{
	m_pHead = (LPBYTE)pBuf;
	m_pData = (LPBYTE)pBuf + MSG_HEADER_SIZE;
}

WORD CMsg::GetSize ()
{
	return *((LPWORD)(m_pHead + 2)) + MSG_HEADER_SIZE;
}

BOOL CMsg::IsReadAll ()
{
	return (GetSize () - MSG_HEADER_SIZE <= m_nRdOff);
}

void CMsg::Copy(LPVOID pBuf)
{
	m_nRdOff = 0;
	m_nWrOff = *((LPWORD)((LPBYTE)pBuf + 2));
	CopyMemory (m_pHead, pBuf, m_nWrOff + MSG_HEADER_SIZE);
}

//////////////////////////////////////////////////////////////////////
// Stream Operators
//////////////////////////////////////////////////////////////////////

CMsg& CMsg::operator<<( const char arg )
{
	WriteData ((LPVOID)&arg, 1);
	return *this;
}

CMsg& CMsg::operator<<( const short arg )
{
	WriteData ((LPVOID)&arg, 2);
	return *this;
}

CMsg& CMsg::operator<<( const int	arg )
{
	WriteData ((LPVOID)&arg, 4);
	return *this;
}

CMsg& CMsg::operator<<( const BYTE arg )
{
	WriteData ((LPVOID)&arg, 1);
	return *this;
}

CMsg& CMsg::operator<<( const WORD arg )
{
	WriteData ((LPVOID)&arg, 2);
	return *this;
}

CMsg& CMsg::operator<<( const DWORD arg )
{
	WriteData ((LPVOID)&arg, 4);
	return *this;
}

CMsg& CMsg::operator<<( const INT64 arg )
{
	WriteData ((LPVOID)&arg, 8);
	return *this;
}

CMsg& CMsg::operator<<( LPCTSTR	arg )
{
	WORD wLength = _tcslen( arg ) + 1;
	int nBufLen = wLength + sizeof(WORD);
	BYTE *pBuf = new BYTE[nBufLen];

	if( pBuf )
	{
		CopyMemory( pBuf, &wLength, sizeof(WORD));
		
		if( nBufLen  - wLength -1 > 0 )
			CopyMemory( pBuf + sizeof(WORD), arg, wLength - 1);
		pBuf[nBufLen - 1] = '\0';
		WriteData((void *) pBuf, nBufLen);

		delete [] pBuf;
	}

	return *this;
}

CMsg& CMsg::operator<<( CMsg* arg )
{
	WriteData ((LPVOID) ((LPBYTE) arg->GetBuf() + MSG_HEADER_SIZE + arg->m_nRdOff ), 
		arg->GetSize() - (MSG_HEADER_SIZE + arg->m_nRdOff));
	return *this;
}

CMsg& CMsg::operator>>( char& arg )
{
	ReadData (&arg, 1);
	return *this;
}

CMsg& CMsg::operator>>( short& arg )
{
	ReadData (&arg, 2);
	return *this;
}

CMsg& CMsg::operator>>( int& arg )
{
	ReadData (&arg, 4);
	return *this;
}

CMsg& CMsg::operator>>( BYTE& arg )
{
	ReadData (&arg, 1);
	return *this;
}

CMsg& CMsg::operator>>( WORD&	arg )
{
	ReadData (&arg, 2);
	return *this;
}

CMsg& CMsg::operator>>( DWORD& arg )
{
	ReadData (&arg, 4);
	return *this;
}

CMsg& CMsg::operator>>( INT64& arg )
{
	ReadData (&arg, 8);
	return *this;
}

/*
CMsg& CMsg::operator>>( CString& arg )
{
	WORD wLength;

	ReadData( (void *) &wLength, sizeof(WORD));
	if (wLength > MSG_DEFAULT_SIZE)
		return *this;

	ReadData( arg.GetBuffer(wLength), wLength);
	arg.ReleaseBuffer();

	return *this;
}
*/

CMsg& CMsg::operator>>(sString& arg)
{
	WORD wLength;
	ReadData( (void *)& wLength, sizeof(WORD));
	if( wLength > MSG_DEFAULT_SIZE)
		return *this;

	arg.resize( wLength);
	
	ReadData( (LPVOID)arg.data(), wLength);

	return *this;
}

CMsg& CMsg::operator=( CMsg& msg )
{
	m_nRdOff = msg.m_nRdOff;
	m_nWrOff = msg.m_nWrOff;
	CopyMemory( m_pHead, msg.m_pHead, MSG_DEFAULT_SIZE);

	return *this;
}

void CMsg::Encrypt(BYTE bKey)
{
	WORD wSize = GetSize();
	BYTE bPrev = 0;
	m_pHead[0] += bPrev + bKey + wSize;
	bPrev = m_pHead[0];
	m_pHead[1] += bPrev + bKey + wSize;
	bPrev = m_pHead[1];
	for (int i = 0; i < wSize - MSG_HEADER_SIZE; i++)
	{
		m_pData[i] += bPrev + bKey + wSize;
		bPrev = m_pData[i];
	}
}

void CMsg::Decrypt(BYTE bKey)
{
	WORD wSize = GetSize();
	BYTE bPrevKey = 0;

	BYTE bPrev = m_pHead[0];
	m_pHead[0] -= bPrevKey + bKey + wSize;
	bPrevKey = bPrev;

	bPrev = m_pHead[1];
	m_pHead[1] -= bPrevKey + bKey + wSize;
	bPrevKey = bPrev;

	for (int i = 0; i < wSize - MSG_HEADER_SIZE; i++)
	{
		bPrev = m_pData[i];
		m_pData[i] -= bPrevKey + bKey + wSize;
		bPrevKey = bPrev;
	}
}
