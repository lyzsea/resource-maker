#pragma once
#include <string>

class CUtilFunc
{
public:
	static void SelectFolder(CEdit& edit_ctrl, HWND m_hWnd);
	static BOOL SelectFile(CWnd* pParent, BOOL bOpen, LPCTSTR pFilters, LPTSTR pFilePath, DWORD dwPathLength);
	static DWORD GetFileSize(const std::string& strFilePath);
};

