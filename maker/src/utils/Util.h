#ifndef  _COMMON_UTIL_HELPER_H
#define _COMMON_UTIL_HELPER_H

#include <string>
#include <windows.h>
#include <vector>

namespace Utils
{
	std::string GetProgramRunDir(HMODULE hDllModule = NULL);  

	void GetFilesFullnameInDir(const std::string &strSourDir, 
		std::string strExtenal,
		std::vector<std::string> &filesPathVector,
		bool isIncludeSubDir);
}

#endif