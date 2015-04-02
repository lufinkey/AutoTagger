
#ifdef _WIN32
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "WebRequest.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <sys/socket.h>
#endif

#ifdef _WIN32
	#pragma comment(lib,"ws2_32.lib")
#endif

namespace autotagger
{
#ifdef _WIN32
	#define close closesocket
#else
	#define SOCKET int
#endif
	
	bool WebRequest_getContentsOfURL(const std::string&host, const std::string&subpage, std::string*results, std::string*error)
	{
		if(results==nullptr)
		{
			if(error!=nullptr)
			{
				*error = "results cannot be null";
			}
			return false;
		}

		#ifdef _WIN32
			WSADATA wsaData;
			int init_result =  WSAStartup(MAKEWORD(2, 2), &wsaData);
			if(init_result != 0)
			{
				if(error!=nullptr)
				{
					*error = "failed to initialize winsock";
				}
				return false;
			}
		#endif

		SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sock < 0)
		{
			if(error!=nullptr)
			{
				*error = "socket";
			}
			return false;
		}
		struct hostent*hostdata = nullptr;
		hostdata = gethostbyname(host.c_str());
		if(hostdata == nullptr)
		{
			if(error!=nullptr)
			{
				*error = "gethostbyname";
			}
			close(sock);
			return false;
		}
		
		struct sockaddr_in sockaddr;
		sockaddr.sin_port = htons(80);
		sockaddr.sin_family = AF_INET;
		memcpy(&sockaddr.sin_addr.s_addr, hostdata->h_addr, sizeof(sockaddr.sin_addr.s_addr));
		if(connect(sock, (struct sockaddr*)(&sockaddr), sizeof(sockaddr)) != 0)
		{
			if(error != nullptr)
			{
				*error = "connect";
			}
			close(sock);
			return false;
		}
		
		std::string request = "GET " + subpage + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
		send(sock, request.c_str(), request.length(), 0);
		char buffer[10000];
		size_t nDataLength;
		while((nDataLength = recv(sock,buffer,9999,0)) > 0)
		{        
			buffer[nDataLength] = '\0';
			results->append(buffer);
		}
		close(sock);

		#ifdef _WIN32
			WSACleanup();
		#endif
		
		return true;
	}

#ifdef _WIN32
	#undef close
#else
	#undef SOCKET
#endif
	
	std::string WebRequest::getContentsOfURL(const std::string&url)
	{
		return "";
	}
	
	void WebRequest::loadContentsOfURL(const std::string&url, LoadFinishCallback callback)
	{
		if(callback != nullptr)
		{
			callback("");
		}
	}
}
