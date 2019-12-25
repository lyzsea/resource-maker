
// makerDlg.h : header file
//

#pragma once


// CmakerDlg dialog
class CmakerDlg : public CDialogEx
{
// Construction
public:
	CmakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CEdit m_editPeFilesDir;
	CEdit m_editLoaderFile;
	CButton m_btnSelectPeDir;
	CButton m_btnSelectLoader;
	CButton m_btnGenerate;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSelectdir();
	afx_msg void OnBnClickedBtnSelectloader();
	afx_msg void OnBnClickedBtnGenerate();
};
