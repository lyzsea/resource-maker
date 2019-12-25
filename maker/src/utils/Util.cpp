#include "Util.h"
//#include <Windows.h>
#include <tchar.h>

namespace Utils
{
	std::string GetProgramRunDir(HMODULE hDllModule)
	{   
		TCHAR exeFullPath[MAX_PATH];
		std::string strPath = _T("");

		GetModuleFileName(hDllModule,exeFullPath,MAX_PATH);
		strPath=(std::string)exeFullPath;
		int pos = strPath.find_last_of('\\', strPath.length());
		return strPath.substr(0, pos+1);
	}

	void GetFilesFullnameInDir(const std::string &strSourDir, 
		std::string strExtenal,
		std::vector<std::string> &filesPathVector,
		bool isIncludeSubDir) 
	{
		std::string strCurDir = strSourDir;//strings::Trim(strSourDir,L"\\");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = ::FindFirstFile((strCurDir + _T("\\*") + strExtenal).c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return;
		}
		do
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string filePath = strCurDir  + _T("\\")+ FindFileData.cFileName;
				filesPathVector.push_back(filePath);
			}
			else if (isIncludeSubDir 
				&& _tcscmp(FindFileData.cFileName,_T(".")) != 0 
				&& _tcscmp(FindFileData.cFileName,_T("..")) != 0)
			{
				GetFilesFullnameInDir(strCurDir + _T("\\")+ FindFileData.cFileName,strExtenal,filesPathVector,isIncludeSubDir);
			}
		}while (FindNextFile(hFind, &FindFileData) != 0);
	}
}