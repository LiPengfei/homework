#ifndef __BLAZER_SABRE_SOCKETWRAPPER_H__
#define __BLAZER_SABRE_SOCKETWRAPPER_H__

#include "Public.h"

#ifdef WIN32
#include <winsock2.h>
#else
#endif

#define BZ_MAX_BACK_LOG_NUM 8

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

INT    BZ_GetSocketLastError();
SOCKET BZ_CreateTCPSocket();
SOCKET BZ_CreateUDPSocket();
SOCKET BZ_CreateListenSocket(CONST CHAR *cszIP, USHORT usPort, INT *pErrorCode = NULL);
BOOL   BZ_CloseSocket(SOCKET &s);

BOOL   BZ_SetSocketNonBlock(SOCKET &s, INT *pErrorCode = NULL);
BOOL   BZ_SetSocketRecvTimeout(SOCKET &s, DWORD dwMilliSeconds, INT *pErrorCode = NULL);
BOOL   BZ_SetSocketSendTimeout(SOCKET &s, DWORD dwMilliSeconds, INT *pErrorCode = NULL);

BZ_DECLARE_NAMESPACE_END

#endif