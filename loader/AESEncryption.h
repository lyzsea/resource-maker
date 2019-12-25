#pragma once
#include <windows.h>

#define DELPOINTER(x) \
	delete x; \
	x = NULL;

class CAESEncryption
{
protected:
	char KeyData[16];

public:
	CAESEncryption(void);
	~CAESEncryption(void);

	//º”√‹
	bool Encryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData, IN OUT DWORD* nSize);

	//Ω‚√‹
	bool Decryption(IN LPVOID lpData, IN DWORD nByte, OUT LPVOID lpOutData);
};
