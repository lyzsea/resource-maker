#ifdef __cplusplus
extern "C"
{
#endif

// �������ֻ���ڼ��������ݵ�CRC32ʱʹ�ã���ʼֵΪ-1�������Ҫȡ��
void 
CRC32(  unsigned long crc,
        unsigned long cbBuffer,
        void * pvBuffer,
        unsigned long * pNewCrc);

// ��ȷ�ĺ���
unsigned long 
GetCRC32(unsigned long cbBuffer,void * pvBuffer);

#ifdef __cplusplus
}
#endif
