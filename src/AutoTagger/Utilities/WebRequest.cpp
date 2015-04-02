
#include "WebRequest.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <sys/socket.h>
#endif

namespace autotagger
{
	static bool WebRequest_initialized = false;

	static int WebRequest_initialize()
	{
		if(!WebRequest_initialized)
		{
			#ifdef _WIN32
				WSADATA wsaData;
				int result =  WSAStartup(MAKEWORD(2, 2), &wsaData);
				if(result == 0)
				{
					WebRequest_initialized = true;
				}
				return result;
			#else
				WebRequest_initialized = true;
				return 0;
			#endif
		}
		return 0;
	}

#ifndef _WIN32
	#define SOCKET int
#endif

	bool WebRequest_getContentsOfURL(const std::string&host, const std::string&subpage, std::string*results)
	{
		return false;
	}

	std::string WebRequest::getContentsOfURL(const std::string&url)
	{
		return "";
	}
	
	void WebRequest::loadContentsOfURL(const std::string&url, LoadFinishCallback callback)
	{
		callback("");
	}
}
