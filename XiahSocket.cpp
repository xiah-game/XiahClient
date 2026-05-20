#include "precompile.h"
#include "xiahsocket.h"
#include "CharacterInfo.h"

#pragma comment(lib,"ws2_32.lib")

using namespace XiahNetwork;

//----------------------------------------------------------------------------------------------------------------------
namespace XiahNetwork
{

XIAH_SOCKET_RECEIVE_FUNCTION				g_XiahSocketReceivedFunction			  = NULL;
XIAH_SOCKET_ON_UNEXPECTEDLY_DISCONNECTED	g_XiahSocketOnUnexpectedlyDisconnected   = NULL;
XIAH_SCOKET_ON_CONNECTED					 g_XiahSocketOnConnected = NULL;
// 어뜩하면 Compile시간을 줄일가 노심초사 하고 있음
// 어차피 Client한테는 Send만 있음 되는데...

class CXiahSocket
{
public:
	CXiahSocket();
	virtual ~CXiahSocket();

	BOOL Connect(LPCTSTR strHostAddr,UINT nPort);
	BOOL Close();
	BOOL IsConnected(){ return m_bConnected;}
	BOOL Send(CMsg &msg);
	BOOL ProcessMessage();
	inline WORD CheckMessage();

protected:
	SOCKET	m_hSocket;
	sString m_strHostAddr;
	int		m_nHostPort;

	int		m_nRead;
	LPBYTE	m_pBuffer;
	CMsg	m_Msg;

	BYTE	m_bKey;

	BOOL m_bConnected;
};

CXiahSocket g_XiahSocket;


//----------------------------------------------------------------------------------------------------------------------
CXiahSocket::CXiahSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_bConnected = FALSE;

	m_nRead = 0;
	m_pBuffer = (LPBYTE)m_Msg.GetBuf();
}

//----------------------------------------------------------------------------------------------------------------------
CXiahSocket::~CXiahSocket()
{
	if( IsConnected())
	{
		Close();
	}
}

//----------------------------------------------------------------------------------------------------------------------
BOOL CXiahSocket::Connect(LPCTSTR strHostAddr,UINT nPort)
{
    DBG_LogFile(_T("CXiahSocket::Connect to %s:%d (m_bConnected=%d)"), strHostAddr, nPort, m_bConnected);
	m_strHostAddr = strHostAddr;
	m_nHostPort = nPort;

	if( m_bConnected)
		return FALSE;

	m_hSocket = socket(AF_INET,SOCK_STREAM,0);
	if( m_hSocket == INVALID_SOCKET) {
        DBG_LogFile(_T("socket() returned INVALID_SOCKET, error: %d"), WSAGetLastError());
		return FALSE;
    }

	struct sockaddr_in serv_addr;
	ZeroMemory( &serv_addr, sizeof( serv_addr));
	serv_addr.sin_family	  = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(strHostAddr);
	serv_addr.sin_port	      = htons( nPort);

	if( serv_addr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost = NULL;
		lphost = gethostbyname(strHostAddr);
		if( lphost != NULL)
			serv_addr.sin_addr.s_addr = ((LPIN_ADDR)lphost)->s_addr;
		else {
            DBG_LogFile(_T("gethostbyname failed for %s, error: %d"), strHostAddr, WSAGetLastError());
			return FALSE;	
        }
	}

	unsigned long argp = 1;
	ioctlsocket( m_hSocket, FIONBIO, &argp);	// none-block socket

    int res = connect( m_hSocket, (struct sockaddr *)&serv_addr, sizeof( serv_addr));
    int initialError = WSAGetLastError(); // Capture immediately!
    DBG_LogFile(_T("connect() returned %d. Socket=%d"), res, m_hSocket);

	if( res == SOCKET_ERROR)
	{
		int nErrorCode = initialError;
        DBG_LogFile(_T("connect() error code: %d (WSAEWOULDBLOCK means async connecting)"), nErrorCode);

		if( nErrorCode != WSAEWOULDBLOCK) // none-block 
		{

			// ¥  
			switch(nErrorCode){
			case WSANOTINITIALISED: DBG_Assert(!"A successful WSAStartup call must occur before using this function."); break;
			case WSAENETDOWN: DBG_Assert(!" The network subsystem has failed. "); break;
			case WSAEADDRINUSE: DBG_Assert(!" The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function. "); break;
			case WSAEINTR: DBG_Assert(!" The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall. "); break;
			case WSAEINPROGRESS: DBG_Assert(!" A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. "); break;
			case WSAEALREADY: DBG_Assert(!" A nonblocking connect call is in progress on the specified socket.\nNote In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll."); break;
 
			case WSAEADDRNOTAVAIL: DBG_Assert(!" The remote address is not a valid address (such as ADDR_ANY). "); break;
			case WSAEAFNOSUPPORT: DBG_Assert(!" Addresses in the specified family cannot be used with this socket. "); break;
			case WSAECONNREFUSED: DBG_Assert(!" The attempt to connect was forcefully rejected. "); break;
			case WSAEFAULT: DBG_Assert(!" The name or the namelen parameter is not a valid part of the user address space, the namelen parameter is too small, or the name parameter contains incorrect address format for the associated address family. "); break;
			case WSAEINVAL: DBG_Assert(!" The parameter s is a listening socket. "); break;
			case WSAEISCONN: DBG_Assert(!" The socket is already connected (connection-oriented sockets only). "); break;
			case WSAENETUNREACH: DBG_Assert(!" The network cannot be reached from this host at this time. "); break;
			case WSAENOBUFS: DBG_Assert(!" No buffer space is available. The socket cannot be connected. "); break;
			case WSAENOTSOCK: DBG_Assert(!" The descriptor is not a socket. "); break;
			case WSAETIMEDOUT: DBG_Assert(!" Attempt to connect timed out without establishing a connection. "); break;
			case WSAEWOULDBLOCK: DBG_Assert(!"  The socket is marked as nonblocking and the connection cannot be completed immediately. "); break;
			case WSAEACCES: DBG_Assert(!" Attempt to connect datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled. "); break;

			}


			Close();
			return FALSE;
		}
	}

	// 일단 붙었다. 

	fd_set fds;
	FD_ZERO( &fds);
	FD_SET(m_hSocket, &fds);
	struct timeval t;

	t.tv_sec = 5;
	t.tv_usec = 0;

	select( 0, NULL, &fds, NULL, &t);

	if( !FD_ISSET( m_hSocket, &fds))
	{
		Close();
		return FALSE;
	}

	m_bConnected = TRUE;

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL CXiahSocket::Close()
{
	if( m_hSocket == INVALID_SOCKET)
		return FALSE;

	shutdown( m_hSocket,0);
	closesocket( m_hSocket);
	m_hSocket = INVALID_SOCKET;

	m_bConnected = false;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL CXiahSocket::Send(CMsg &msg)
{
	msg.Encrypt( m_bKey);

	int r = send(m_hSocket, (char *)msg.GetBuf(),msg.GetSize(), 0);

	if( r == SOCKET_ERROR)
	{
		int nErrorCode = WSAGetLastError();

			// 진짜 에러 났네
		switch(nErrorCode){
		case WSANOTINITIALISED: DBG_Assert(!"A successful WSAStartup call must occur before using this function."); break;
		case WSAENETDOWN: DBG_Assert(!" The network subsystem has failed. "); break;
		case WSAEADDRINUSE: DBG_Assert(!" The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function. "); break;
		case WSAEINTR: DBG_Assert(!" The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall. "); break;
		case WSAEINPROGRESS: DBG_Assert(!" A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. "); break;
		case WSAEALREADY: DBG_Assert(!" A nonblocking connect call is in progress on the specified socket.\nNote In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll."); break;

		case WSAEADDRNOTAVAIL: DBG_Assert(!" The remote address is not a valid address (such as ADDR_ANY). "); break;
		case WSAEAFNOSUPPORT: DBG_Assert(!" Addresses in the specified family cannot be used with this socket. "); break;
		case WSAECONNREFUSED: DBG_Assert(!" The attempt to connect was forcefully rejected. "); break;
		case WSAEFAULT: DBG_Assert(!" The name or the namelen parameter is not a valid part of the user address space, the namelen parameter is too small, or the name parameter contains incorrect address format for the associated address family. "); break;
		case WSAEINVAL: DBG_Assert(!" The parameter s is a listening socket. "); break;
		case WSAEISCONN: DBG_Assert(!" The socket is already connected (connection-oriented sockets only). "); break;
		case WSAENETUNREACH: DBG_Assert(!" The network cannot be reached from this host at this time. "); break;
		case WSAENOBUFS: DBG_Assert(!" No buffer space is available. The socket cannot be connected. "); break;
		case WSAENOTSOCK: DBG_Assert(!" The descriptor is not a socket. "); break;
		case WSAETIMEDOUT: DBG_Assert(!" Attempt to connect timed out without establishing a connection. "); break;
		case WSAEWOULDBLOCK: DBG_Assert(!"  The socket is marked as nonblocking and the connection cannot be completed immediately. "); break;
		case WSAEACCES: DBG_Assert(!" Attempt to connect datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled. "); break;
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
inline WORD CXiahSocket::CheckMessage()
{
	if( m_nRead < MSG_HEADER_SIZE)
		return 0;
	
	WORD wMsgSize = m_Msg.GetSize();

	if( MSG_DEFAULT_SIZE < wMsgSize)
		return 0;

	if( m_nRead < wMsgSize)
		return 0;

	return wMsgSize;
}


//----------------------------------------------------------------------------------------------------------------------
BOOL CXiahSocket::ProcessMessage()
{
	if(	!IsConnected())
		return FALSE;

	// Send keepalive heartbeat every 3 seconds to prevent NAT/firewall idle timeout
	static DWORD s_dwLastKeepAlive = 0;
	DWORD dwNow = GetTickCount();
	if (dwNow - s_dwLastKeepAlive > 3000) {
		s_dwLastKeepAlive = dwNow;
		char keepAlive[6] = {0, 0, 0, 0, 0, 0};
		send(m_hSocket, keepAlive, 6, 0);
	}

	timeval t;
	fd_set fdr;

	t.tv_sec = 0;
	t.tv_usec = 1000;
	FD_ZERO( &fdr);
	FD_SET( m_hSocket, &fdr);

	if( ::select( 0, &fdr, NULL, NULL, &t) == SOCKET_ERROR)
		return FALSE;

	if( FD_ISSET( m_hSocket, &fdr))
	{
		int nRead = recv( m_hSocket, (char *)(m_pBuffer + m_nRead), MSG_DEFAULT_SIZE - m_nRead, 0);

		if( nRead > 0)
		{
			m_nRead += nRead;
			WORD wMsgSize;

			while( (wMsgSize = CheckMessage()) > 0)
			{
				DBG_Assert( g_XiahSocketReceivedFunction);

				if( !m_Msg.ID())
				{
					m_Msg >> m_bKey;
					g_XiahSocketOnConnected();

					DBG_Put(_T("m_bKey = %d\n"),m_bKey);
				}
				else
				{
					m_Msg.Decrypt( m_bKey);
				
					g_XiahSocketReceivedFunction( m_Msg);
				}

				m_Msg.Clear();
				m_nRead -= wMsgSize;

				if( m_nRead)
				{
					memmove( m_pBuffer, m_pBuffer + wMsgSize, m_nRead);
					memset(m_pBuffer + m_nRead, 0 ,MSG_DEFAULT_SIZE - m_nRead);
				}
			}
		}
		else	// 연결이 해제 되었거나, 먼가 문제가 있다
		{
			DBG_Assert( g_XiahSocketOnUnexpectedlyDisconnected);

			g_XiahSocketOnUnexpectedlyDisconnected();

			Close();

		}
	}

	return TRUE;
}


//----------------------------------------------------------------------------------------------------------------------
BOOL InitializeXiahClientSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 1, 1);
	
	if( WSAStartup( wVersionRequested, &wsaData) != 0)
		return FALSE;	// 크헐

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL UnitializeXiahClientSocket()
{
	WSACleanup();

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL ConnectToServer(LPCTSTR server_address,int nPort)
{
	DBG_Assert( !g_XiahSocket.IsConnected());

	return g_XiahSocket.Connect( server_address, nPort);
}

BOOL DisconnectFromServer()
{
	return g_XiahSocket.Close();
}

//----------------------------------------------------------------------------------------------------------------------
BOOL SendNetMsg(CMsg &msg)
{
	DBG_Assert( g_XiahSocket.IsConnected());
#ifndef _DEBUG
	if( !g_XiahSocket.IsConnected())
		return FALSE;
#endif
	
	if( g_MainCharInfo.m_bInteractionFlag)
		return FALSE;

	return g_XiahSocket.Send( msg);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
BOOL ProcessNetworkMessage()
{
	return g_XiahSocket.ProcessMessage();
}


};
