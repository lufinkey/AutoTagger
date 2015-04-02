
#pragma once

#include <string>

namespace autotagger
{
	class WebRequest
	{
	public:
		typedef void(*LoadFinishCallback)(const std::string&);
		
		static std::string getContentsOfURL(const std::string&url);
	};
}