#pragma once

#include <fmt/format.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock2.h>
extern WSADATA wsaData;
#define errno WSAGetLastError()
wVersionRequested = MAKEWORD(2, 2);
WSAStartup(wVersionRequested, &wsaData);
#include <thread.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define MAXPACKETSIZE 4096
#define MAXSOCKETS 64

#ifdef NDEBUG
#define _EXPECT2(Cond, Str) Cond
#define _EXPECT3(Cond, Str, Return) Cond
#define _GET_MACRO_EXPECT(_1, _2, _3, NAME, ...) NAME
#define Expect(...) _GET_MACRO_EXPECT(__VA_ARGS__, _EXPECT3, _EXPECT2)(__VA_ARGS__)
#else
#define _EXPECT2(Cond, Str) if (!(Cond)) fmt::print("Error: {0}, File: {1}, Line: {2}\n", Str, __FILE__, __LINE__)
#define _EXPECT3(Cond, Str, Return) if (!(Cond)) {fmt::print("Error: {0}, File: {1}, Line: {2}\n", Str, __FILE__, __LINE__); return Return;}
#define _GET_MACRO_EXPECT(_1, _2, _3, NAME, ...) NAME
#define Expect(...) _GET_MACRO_EXPECT(__VA_ARGS__, _EXPECT3, _EXPECT2)(__VA_ARGS__)
#endif


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
		if (!mAddress.sin_addr.s_addr)
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

class Data
{
private:
	char *data;
	size_t length;
	bool delFlg;

public:
	Data() : data(), length(0), delFlg(false) {}
	//Data(char *_data) : data(_data), length(strlen(_data)), delFlg(false) {}
	Data(const std::string &str) : data(new char[str.length()]), length(str.length()), delFlg(true) {strncpy(data, str.c_str(), str.length());}

	~Data()
	{
		if (delFlg)
			delete [] data;
	}

	void setStr(const std::string &str)
	{
		data = new char[str.length()];
		strncpy(data, str.c_str(), str.length());
		length = str.length();
		delFlg = true;
	}

	void init()
	{
		data = new char[length];
		delFlg = true;
	}

	char* getHeader() const
	{
		return (char*)&length;
	}

	size_t getHeaderLen()
	{
		return sizeof(length);
	}

	char* getData()
	{
		return data;
	}

	size_t& getDataLen()
	{
		return length;
	}

	std::string getDataStr() const
	{
		return std::string(data);
	}

	char* toString() const
	{
		return data;
	}
};

inline int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds, timeval* timeout)
{
	return ::select(nfds, readfds, writefds, errorfds, timeout);
}
	
inline int accept(int socket, TCP::Address& address)
{
	return ::accept(socket, (sockaddr*)&address.getSockAddr(), &address.getAddrLen());
}

inline ssize_t send(int socket, Data &data, int flags = 0)
{
	::send(socket, data.getHeader(), data.getHeaderLen(), flags);
	return ::send(socket, data.getData(), data.getDataLen(), flags);
}

inline ssize_t recv(int socket, Data &data, int flags = 0)
{
	::recv(socket, data.getHeader(), data.getHeaderLen(), flags);
	data.init();
	return ::recv(socket, data.getData(), data.getDataLen(), flags);
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

inline int connect(int socket, TCP::Address& address)
{
	return ::connect(socket, (sockaddr*)&address.getSockAddr(), address.getAddrLenConst());
}
};
