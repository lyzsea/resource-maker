#include "Rijndael.h"
#include ".\AESEncryption.h"

CAESEncryption::CAESEncryption(void)
{
	memset(KeyData, 0, 16);
	memcpy(KeyData,"1234567890123456", 16);
/*	调试
	char In[32];
	char Out[32];
	memcpy(In,"a\0aaaaaaaaaaaaaba\0aaaaaaaaaaaaab",32);
	DWORD nSize = 32;
	Encryption(In,32,Out,&nSize);	
	memset(In, 0, 32);
	Decryption(Out, 32, In);
*/
}

CAESEncryption::~CAESEncryption(void)
{
}

//加密函数
bool CAESEncryption::Encryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData, IN OUT DWORD* nSize)
{
	CRijndael aes;
	char* lpInData = NULL;
	char lpInBlock[16] = {0};
	char lpOutBlock[16] = {0};
	DWORD nInBufferSize = nByte % 16;
	DWORD nBlockCount = 0;
	char* lpOutPointer = NULL;

	//计算大小，开辟16的倍数的内存
	if (nInBufferSize != 0) {
		nInBufferSize = ((nByte / 16) + 1) * 16;
	} else {
		nInBufferSize = nByte;
	}

	nBlockCount = nInBufferSize / 16;

	//计算输出缓冲区是否够大
	if (*nSize < nInBufferSize) {
		*nSize = nInBufferSize;
		return false;
	}

	*nSize = nInBufferSize;

	//拷贝需要加密的数据
	lpInData = new char[nInBufferSize];
	memset(lpInData, 0, nInBufferSize);
	memcpy(lpInData, lpData, nByte);

	memset(lpOutData, 0, *nSize);
	lpOutPointer = (char*)lpOutData;

	//设置加密密钥
	aes.MakeKey(KeyData, CRijndael::sm_chain0);

	//分块加密
	for (DWORD i = 0; i < nBlockCount; i++)
	{
		memset(lpInBlock, 0, 16);
		memset(lpOutBlock, 0, 16);
		memcpy(lpInBlock, lpInData + i * 16, 16);
		aes.EncryptBlock(lpInBlock, lpOutBlock);
		
		//拷贝到输出缓冲区
		memcpy(lpOutPointer, lpOutBlock, 16);

		//移动指针
		lpOutPointer += 16;
	}

	return true;
}

//解密函数
bool CAESEncryption::Decryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData)
{
	CRijndael aes;
	DWORD n = nByte % 16;
	char cInBlock[16];
	char cOutBlock[16];
	char* lpOutPointer = NULL;
	
	//判断缓冲区长度是否符合 16 的倍数
	if (n != 0) {
		return false;
	}

	//计算块的数量
	DWORD nBlockCount = nByte / 16;

	//输出指针
	lpOutPointer = (char*)lpOutData;

	//设置加密密钥
	aes.MakeKey(KeyData, CRijndael::sm_chain0);

	//分块解密
	for (DWORD i = 0; i < nBlockCount ; i++)
	{
		memset(cInBlock, 0, 16);
		memset(cOutBlock, 0, 16);

		memcpy(cInBlock, (LPBYTE)lpData + i * 16, 16);
		aes.DecryptBlock(cInBlock, cOutBlock);

		memcpy(lpOutPointer, cOutBlock, 16);
		lpOutPointer += 16;
	}

	return true;
}