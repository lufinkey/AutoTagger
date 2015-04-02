
#pragma once

#include <string>

namespace autotagger
{
	class WebRequest
	{
	public:
		typedef void(*LoadFinishCallback)(const std::string&);
		
		static void separateURL(const std::string&url, std::string*host, std::string*subpage);
		static std::string getContentsOfURL(const std::string&url);
	};
}
