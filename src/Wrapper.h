#include <iostream>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock.h>
extern WSADATA wsaData;
//WSAStartup(0x0202, &wsaData);
//typedef SSIZE_T ssize_t;
#include <thread.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif


#ifndef WRAPPER_H
#define WRAPPER_H


#define _EXPECT2(Cond, Str) if (!(Cond)) printf("Error: %s, File: %s, Line: %d\n", Str, __FILE__, __LINE__)
#define _EXPECT3(Cond, Str, Return) if (!(Cond)) {printf("Error: %s, File: %s, Line: %d\n", Str, __FILE__, __LINE__); return Return;}
#define _GET_MACRO_EXPECT(_1, _2, _3, NAME, ...) NAME
#define Expect(...) _GET_MACRO_EXPECT(__VA_ARGS__, _EXPECT3, _EXPECT2)(__VA_ARGS__)


namespace TCP
{
struct Address
{
	sockaddr_in mAddress;
	socklen_t mAddrLen = sizeof(sockaddr_in);

	Address()
	{
	}

	Address(const char* address, const int port)
	{
		setup(address, port);
	}

	void setup(const char* address, const int port)
	{
		memset(&mAddress, 0, sizeof(sockaddr_in));
		mAddress.sin_family = AF_INET;
		mAddress.sin_addr.s_addr = inet_addr(address);
		if (mAddress.sin_addr.s_addr == -1)
		{
			hostent* he = ::gethostbyname(address);
			Expect(he != NULL, "Host not recognized");
			mAddress.sin_addr = *((in_addr**)he->h_addr_list)[0];
		}
		mAddress.sin_port = htons(port);
	}

	sockaddr_in& getSockAddr()
	{
		return mAddress;
	}

	const char* getAddrStr() const
	{
		return inet_ntoa(mAddress.sin_addr);
	}

	int getPort() const
	{
		return ntohs(mAddress.sin_port);
	}

	socklen_t& getAddrLen()
	{
		return mAddrLen;
	}

	socklen_t getAddrLenConst() const
	{
		return mAddrLen;
	}
};

inline int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds, timeval* timeout)
{
	return ::select(nfds, readfds, writefds, errorfds, timeout);
}
	
inline int accept(int socket, sockaddr* address, socklen_t* address_len)
{
	return ::accept(socket, address, address_len);
}

inline int accept(int socket, TCP::Address& address)
{
	return ::accept(socket, (sockaddr*)&address.getSockAddr(), &address.getAddrLen());
}

inline ssize_t send(int socket, const char *buffer, size_t length, int flags)
{
	::send(socket, (char*)&length, sizeof(length), flags);
	return ::send(socket, buffer, length, flags);
}

inline ssize_t recv(int socket, char *buffer, size_t &length, int flags)
{
	::recv(socket, (char*)&length, sizeof(length), flags);
	return ::recv(socket, buffer, length, flags);
}

inline int getpeername(int socket, sockaddr *address, socklen_t *address_len)
{
	return ::getpeername(socket, address, address_len);
}

inline int getpeername(int socket, TCP::Address& address)
{
	return ::getpeername(socket, (sockaddr*)&address.getSockAddr(), &address.getAddrLen());
}

inline int close(int fildes)
{
#ifdef _WIN32
	closesocket(fildes);
	WSACleanup();
	return 0;
#else
	return ::close(fildes);
#endif
}

inline int socket(int domain, int type, int protocol)
{
	return ::socket(domain, type, protocol);
}

inline int bind(int socket, const sockaddr *address, socklen_t address_len)
{
	return ::bind(socket, address, address_len);
}

inline int bind(int socket, TCP::Address& address)
{
	return ::bind(socket, (sockaddr*)&address.getSockAddr(), address.getAddrLenConst());
}

inline int listen(int socket, int backlog)
{
	return ::listen(socket, backlog);
}

inline hostent* gethostbyname(const char *name)
{
	return ::gethostbyname(name);
}

inline int connect(int socket, const sockaddr *address, socklen_t address_len)
{
	return ::connect(socket, address, address_len);
}

inline int connect(int socket, TCP::Address& address)
{
	return ::connect(socket, (sockaddr*)&address.getSockAddr(), address.getAddrLenConst());
}
};


#endif
