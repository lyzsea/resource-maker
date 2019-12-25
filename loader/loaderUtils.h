#include <string>
#include <windows.h>

class CLoaderUtils
{
public:
	static BOOL Execute(LPCTSTR lpszProcessName, LPTSTR lpszCmdLine, LPCTSTR lpCurrentDirectory, BOOL bForceAdmin, BOOL bWaitProcess);

	static bool IsDirectory(const TCHAR* filename);

	static std::wstring GetProgramRunDir(HMODULE hDllModule);

	static std::string  ws2s(const std::wstring& s_src, UINT CodePage = CP_UTF8);
};
