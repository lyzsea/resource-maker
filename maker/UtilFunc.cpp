#include "stdafx.h"
#include "UtilFunc.h"
#include "utils/Util.h"


bool CUtilFunc::IsDirectory(const TCHAR* filename)
{
	DWORD dwAttr = ::GetFileAttributes(filename);  //�õ��ļ�����
	if (dwAttr == 0xFFFFFFFF)    // �ļ���Ŀ¼������
		return false;
	else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)  // �����Ŀ¼
		return true;
	else
		return false;

}

void CUtilFunc::SelectFolder(CEdit& edit_ctrl, HWND m_hWnd)
{
	// TODO: Add your control notification handler code here
	TCHAR tcPath[MAX_PATH] = { 0 };     //���ѡ���Ŀ¼·�� 
	CString str;
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = tcPath;
	bi.lpszTitle = _T("Please select a directory ��");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, tcPath))
	{
		edit_ctrl.SetWindowText(tcPath);
	}
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


