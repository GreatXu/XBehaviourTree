
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <sys/time.h>
#endif // _WIN32

#include "Common/public/qc_util.h"

static unsigned long timeStartSecond = 0;
static struct timeval curtime;

void initUtilLib()
{
#ifdef _WIN32
	static int initsock = 0;
	if (initsock == 0)
	{
		WORD socketVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(socketVersion, &wsaData) != 0)
		{
			return;
		}
		initsock = 1;
	}

	timeStartSecond = GetTickCount();
#else
	gettimeofday(&curtime, NULL);
	timeStartSecond = curtime.tv_sec;
#endif
}

quint iclock()
{
#ifdef _WIN32 // _WIN32
	return (unsigned int)(GetTickCount() - timeStartSecond);
#else
	gettimeofday(&curtime, NULL);
	return (unsigned int)((curtime.tv_sec - timeStartSecond) * 1000 + curtime.tv_usec / 1000);
#endif
}

#ifdef _WIN32
void usleep(qint64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

inline quint _min(quint a, quint b)
{
	return a <= b ? a : b;
}

inline quint _max(quint a, quint b)
{
	return a >= b ? a : b;
}

inline quint _bound(quint lower, quint mid, quint upper)
{
	return _min(_max(lower, mid), upper);
}

inline quint _abs(qint a)
{
	return a >= 0 ? a : -a;
}

inline void putUint16(quint8 *b, quint v)
{
	b[0] = (quint8)(v);
	b[1] = (quint8)(v >> 8);
}

inline void putUint(quint8 *b, quint v)
{
	b[0] = (quint8)(v);
	b[1] = (quint8)(v >> 8);
	b[2] = (quint8)(v >> 16);
	b[3] = (quint8)(v >> 24);
}

inline quint readUint(const quint8 *data)
{
	return (quint)data[0] | ((quint)data[1] << 8) | ((quint)(data[2]) << 16) | ((quint)(data[3]) << 24);
}

#endif // _WIN32