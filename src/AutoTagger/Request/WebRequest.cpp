
#ifdef _WIN32
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "WebRequest.h"

#include <iostream>
#include <stdexcept>
#include <thread>

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
		
		results->clear();
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
	
	void WebRequest::separateURL(const std::string&url, std::string*host, std::string*subpage)
	{
		if(host == nullptr)
		{
			throw std::invalid_argument("host");
		}
		else if(subpage == nullptr)
		{
			throw std::invalid_argument("subpage");
		}
		
		size_t host_start_index = 0;
		size_t subpage_start_index = url.length();
		
		std::string protocol_separator = "://";
		
		std::string protocol = "http";
		size_t protocol_end_index = url.find(protocol_separator);
		if(protocol_end_index != std::string::npos)
		{
			protocol = url.substr(0, protocol_end_index-0);
			host_start_index = protocol_end_index + protocol_separator.length();
		}
		
		if(protocol.compare("http")!=0 && protocol.length()!=0)
		{
			if(protocol.compare("https") == 0)
			{
				std::cerr << "https is not yet supported" << std::endl;
			}
			throw std::invalid_argument("url");
		}
		
		size_t next_slash_index = url.find("/", host_start_index);
		if(next_slash_index == std::string::npos)
		{
			next_slash_index = url.find("?", host_start_index);
		}
		
		if(next_slash_index != std::string::npos)
		{
			subpage_start_index = next_slash_index;
		}
		
		*host = url.substr(host_start_index, subpage_start_index);
		*subpage = url.substr(subpage_start_index, url.length());
		if(subpage->length() == 0)
		{
			*subpage = "/";
		}
	}
	
	std::string WebRequest::getContentsOfURL(const std::string&url)
	{
		std::string host;
		std::string subpage;
		try
		{
			WebRequest::separateURL(url, &host, &subpage);
		}
		catch(const std::invalid_argument&)
		{
			return "";
		}
		
		std::string results;
		std::string error;
		bool success = WebRequest_getContentsOfURL(host, subpage, &results, &error);
		
		return results;
	}
}
