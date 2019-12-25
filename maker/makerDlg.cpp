
// makerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "maker.h"
#include "makerDlg.h"
#include "afxdialogex.h"
#include "Utils/Util.h"
#include "UtilFunc.h"
#include "zip/zip.h"
#include "aes/AESEncryption.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmakerDlg dialog



CmakerDlg::CmakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FilesDir, m_editPeFilesDir);
	DDX_Control(pDX, IDC_EDIT_LoaderPath, m_editLoaderFile);
	DDX_Control(pDX, IDC_BTN_SelectDir, m_btnSelectPeDir);
	DDX_Control(pDX, IDC_BTN_SelectLoader, m_btnSelectLoader);
	DDX_Control(pDX, IDC_BTN_Generate, m_btnGenerate);
}

BEGIN_MESSAGE_MAP(CmakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SelectDir, &CmakerDlg::OnBnClickedBtnSelectdir)
	ON_BN_CLICKED(IDC_BTN_SelectLoader, &CmakerDlg::OnBnClickedBtnSelectloader)
	ON_BN_CLICKED(IDC_BTN_Generate, &CmakerDlg::OnBnClickedBtnGenerate)
END_MESSAGE_MAP()


// CmakerDlg message handlers

BOOL CmakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmakerDlg::OnBnClickedBtnSelectdir()
{
	// TODO: Add your control notification handler code here
	CUtilFunc::SelectFolder(m_editPeFilesDir, m_hWnd);
}


void CmakerDlg::OnBnClickedBtnSelectloader()
{
	// TODO: Add your control notification handler code here
	TCHAR tcPath[MAX_PATH] = { 0 };
	if (CUtilFunc::SelectFile(this, TRUE, _T("input File(*.exe)|*.EXE|All Files (*.*)|*.*||"), tcPath, MAX_PATH)
		&& CUtilFunc::IsDirectory(tcPath))
	{
		m_editLoaderFile.SetWindowText(tcPath);
	}
	else 
	{
		AfxMessageBox(_T("Invalid directory，please reselect!"));
	}
}


void CmakerDlg::OnBnClickedBtnGenerate()
{
	// TODO: Add your control notification handler code here
	TCHAR tcPeDir[MAX_PATH] = { 0 };
	TCHAR tcLoaderPath[MAX_PATH] = { 0 };
	m_editPeFilesDir.GetWindowText(tcPeDir, MAX_PATH);
	m_editLoaderFile.GetWindowText(tcLoaderPath, MAX_PATH);
	
	if (!PathFileExists(tcLoaderPath))
	{
		MessageBox(_T("payloader do not exist,please check it!"), _T("maker"), MB_OK);
		return;
	}
	std::vector<std::string> vecFiles;
	Utils::GetFilesFullnameInDir(tcPeDir, _T(".*"), vecFiles, false);
	if (vecFiles.empty())
	{
		MessageBox(_T("input directory is empty, please chect it!"), _T("maker"), MB_OK);
		return;
	}
	m_btnGenerate.EnableWindow(FALSE);
	DWORD dwTotalSize = 0;
	for (size_t i = 0; i < vecFiles.size(); ++i)
	{
		dwTotalSize += CUtilFunc::GetFileSize(vecFiles[i].c_str());
	}

	void* pData = NULL;
	HZIP hZip = CreateZip(0, dwTotalSize, NULL);
	for (size_t i = 0; i < vecFiles.size(); ++i)
	{
		std::string fileName = PathFindFileName(vecFiles[i].c_str());
		ZipAdd(hZip, fileName.c_str(), vecFiles[i].c_str());
	}
	DWORD dwZipSize = 0;
	ZipGetMemory(hZip, &pData, &dwZipSize);

	CAESEncryption aes;
	DWORD eSize = 0;
	aes.Encryption(NULL, dwZipSize, NULL, &eSize);
	char* lpData = new char[eSize];
	memset(lpData, 0, eSize);
	aes.Encryption(pData, dwZipSize, lpData, &eSize);

	//替换资源 magic 为压缩好的ZIP数据
	std::string copyLoaderFile = tcLoaderPath;
	copyLoaderFile = copyLoaderFile.substr(0, copyLoaderFile.find_last_of("."));
	copyLoaderFile.append("_generate.exe");
	CopyFile(tcLoaderPath, copyLoaderFile.c_str(), TRUE);
	HANDLE hHandle = BeginUpdateResource(copyLoaderFile.c_str(), false);
	UpdateResource(hHandle, _T("magic"), MAKEINTRESOURCE(130), 2052, lpData, eSize);
	EndUpdateResource(hHandle, FALSE);

	//关闭ZIP文件句柄
	CloseZip(hZip);
	delete[] lpData;
	lpData = NULL;
	MessageBox(_T("generate success!"), _T("maker"), MB_OK);
	m_btnGenerate.EnableWindow(TRUE);
}
