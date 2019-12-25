#include "stdafx.h"
#include "UtilFunc.h"
#include "utils/Util.h"


void CUtilFunc::SelectFolder(CEdit& edit_ctrl, HWND m_hWnd)
{
	// TODO: Add your control notification handler code here
	TCHAR tcPath[MAX_PATH] = { 0 };     //存放选择的目录路径 
	CString str;
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = tcPath;
	bi.lpszTitle = _T("Please select a directory ：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, tcPath))
	{
		edit_ctrl.SetWindowText(tcPath);
	}
	else
		AfxMessageBox(_T("Invalid directory，please reselect!"));
}

BOOL CUtilFunc::SelectFile(CWnd* pParent, BOOL bOpen, LPCTSTR pFilters, LPTSTR pFilePath, DWORD dwPathLength)
{
	CFileDialog *pFileDlg = new CFileDialog(bOpen, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, pFilters, pParent);
	if (pFileDlg == NULL)
		return FALSE;

	BOOL bRet = FALSE;
	if (pFileDlg->DoModal() == IDOK)
	{
		_tcscpy_s(pFilePath, dwPathLength, pFileDlg->GetPathName());
		bRet = TRUE;
	}

	delete pFileDlg;

	return bRet;
}

DWORD CUtilFunc::GetFileSize(const std::string& strFilePath)
{
	CFileStatus status;
	CFile::GetStatus(strFilePath.c_str(), status, NULL);

	return status.m_size;          
}


