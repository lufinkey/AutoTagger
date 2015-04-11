
#include "AutoTagger/AutoTagger.h"
#include <dirent.h>

using namespace autotagger;

std::vector<std::string> getFilesFromDirectory(const std::string&path)
{
	std::vector<std::string> files;
	DIR*dir = opendir(path.c_str());
	if(dir==NULL)
	{
		return;
	}
	struct dirent *entry = readdir(dir);
	while (entry != NULL)
	{
		std::string name = entry->d_name;
		if(name!="." && name!="..")
		{
			if(entry->d_type == DT_REG)
			{
				files.push_back(name);
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return files;
}

int main(int argc, char* argv[])
{
	system("PAUSE");
	return 0;
}

