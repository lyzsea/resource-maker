#include "stdafx.h"
#include "loaderUtils.h"

#include <ShellAPI.h>

bool CLoaderUtils::IsDirectory(const TCHAR* filename)
{
	DWORD dwAttr = ::GetFileAttributes(filename);  //得到文件属性
	if (dwAttr == 0xFFFFFFFF)    // 文件或目录不存在
		return false;
	else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)  // 如果是目录
		return true;
	else
		return false;

}

std::wstring CLoaderUtils::GetProgramRunDir(HMODULE hDllModule)
{
	TCHAR exeFullPath[MAX_PATH];
	std::wstring strPath = _T("");

	GetModuleFileName(hDllModule, exeFullPath, MAX_PATH);
	strPath = (std::wstring)exeFullPath;
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos + 1);
}

std::string CLoaderUtils::ws2s(const std::wstring& wide, UINT CodePage)
{
	int wide_length = static_cast<int>(wide.length());
	if (wide_length == 0)
		return std::string();

	// Compute the length of the buffer we'll need.
	int charcount = WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
		NULL, 0, NULL, NULL);
	if (charcount == 0)
		return std::string();

	std::string mb;
	mb.resize(charcount);
	WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
		&mb[0], charcount, NULL, NULL);

	return mb;
}

BOOL CLoaderUtils::Execute(LPCTSTR lpszProcessName, LPTSTR lpszCmdLine, LPCTSTR lpCurrentDirectory, BOOL bForceAdmin, BOOL bWaitProcess)
{
	BOOL bRet = FALSE;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	DWORD dwErrCode = 0;
	HANDLE hProcess = NULL;
	TCHAR szCmd[MAX_PATH * 2] = { 0 };

	if (bForceAdmin)
	{
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.hwnd = NULL;
		sei.lpVerb = _T("runas");
		sei.lpFile = lpszProcessName;
		sei.lpParameters = (LPWSTR)(LPCWSTR)lpszCmdLine;
		sei.nShow = SW_HIDE;
		sei.lpDirectory = lpCurrentDirectory;

		bRet = ::ShellExecuteEx(&sei);

		hProcess = sei.hProcess;
	}
	else
	{
		memset(&si, 0, sizeof(si));

		si.cb = sizeof(STARTUPINFO);
		si.lpReserved = NULL;
		si.lpTitle = NULL;
		si.lpDesktop = NULL;
		si.dwX = 0;
		si.dwY = 0;
		si.dwXSize = 0;
		si.dwYSize = 0;
		si.dwXCountChars = 0;
		si.dwYCountChars = 0;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;   //SW_SHOWDEFAULT; 
		si.lpReserved2 = NULL;
		si.cbReserved2 = 0;
		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

		_stprintf_s(szCmd, _T("%s %s"), lpszProcessName, lpszCmdLine);

		bRet = ::CreateProcess(
			NULL,
			szCmd,
			NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, lpCurrentDirectory, &si, &processInfo
			);

		if (bRet)
		{
			::CloseHandle(processInfo.hThread);
			hProcess = processInfo.hProcess;
		}
		else
		{
			dwErrCode = GetLastError();
		}
	}

	if (bRet)
	{
		if (bWaitProcess)
		{
			::WaitForSingleObject(hProcess, INFINITE);
		}
		::CloseHandle(hProcess);
	}

	return bRet;
}