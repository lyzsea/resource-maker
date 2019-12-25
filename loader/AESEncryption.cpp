#include "Rijndael.h"
#include ".\AESEncryption.h"

CAESEncryption::CAESEncryption(void)
{
	memset(KeyData, 0, 16);
	memcpy(KeyData,"1234567890123456", 16);
/*	����
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

//���ܺ���
bool CAESEncryption::Encryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData, IN OUT DWORD* nSize)
{
	CRijndael aes;
	char* lpInData = NULL;
	char lpInBlock[16] = {0};
	char lpOutBlock[16] = {0};
	DWORD nInBufferSize = nByte % 16;
	DWORD nBlockCount = 0;
	char* lpOutPointer = NULL;

	//�����С������16�ı������ڴ�
	if (nInBufferSize != 0) {
		nInBufferSize = ((nByte / 16) + 1) * 16;
	} else {
		nInBufferSize = nByte;
	}

	nBlockCount = nInBufferSize / 16;

	//��������������Ƿ񹻴�
	if (*nSize < nInBufferSize) {
		*nSize = nInBufferSize;
		return false;
	}

	*nSize = nInBufferSize;

	//������Ҫ���ܵ�����
	lpInData = new char[nInBufferSize];
	memset(lpInData, 0, nInBufferSize);
	memcpy(lpInData, lpData, nByte);

	memset(lpOutData, 0, *nSize);
	lpOutPointer = (char*)lpOutData;

	//���ü�����Կ
	aes.MakeKey(KeyData, CRijndael::sm_chain0);

	//�ֿ����
	for (DWORD i = 0; i < nBlockCount; i++)
	{
		memset(lpInBlock, 0, 16);
		memset(lpOutBlock, 0, 16);
		memcpy(lpInBlock, lpInData + i * 16, 16);
		aes.EncryptBlock(lpInBlock, lpOutBlock);
		
		//���������������
		memcpy(lpOutPointer, lpOutBlock, 16);

		//�ƶ�ָ��
		lpOutPointer += 16;
	}

	return true;
}

//���ܺ���
bool CAESEncryption::Decryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData)
{
	CRijndael aes;
	DWORD n = nByte % 16;
	char cInBlock[16];
	char cOutBlock[16];
	char* lpOutPointer = NULL;
	
	//�жϻ����������Ƿ���� 16 �ı���
	if (n != 0) {
		return false;
	}

	//����������
	DWORD nBlockCount = nByte / 16;

	//���ָ��
	lpOutPointer = (char*)lpOutData;

	//���ü�����Կ
	aes.MakeKey(KeyData, CRijndael::sm_chain0);

	//�ֿ����
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