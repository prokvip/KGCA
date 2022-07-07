#include "TFileIO.h"
//Consider using fopen_s instead
size_t   TFileIO::Write(void const* pData,
					 int iSize)
{
	if (m_pStream == nullptr) return -1;
	size_t size = fwrite(pData, iSize, 1, m_pStream);
	return size;
}
bool TFileIO::CreateFile(const  char* pFileName)
{
	if (m_pStream != nullptr) return false;
	//m_pStream = fopen(pFileName, "wb");
	int iError = fopen_s(&m_pStream, pFileName, "wb");
	if (m_pStream == nullptr) return false;
	return true;
}
size_t   TFileIO::Read(void* pData,
	int iSize)
{
	if (m_pStream == nullptr) return -1;
	size_t size = fread(pData, iSize, 1, m_pStream);
	return size;
}
bool TFileIO::OpenFile(const  char* pFileName)
{
	if (m_pStream != nullptr) return false;
	int iError = fopen_s(&m_pStream, pFileName, "rb");
	if (m_pStream == nullptr) return false;
	return true;
}
void     TFileIO::CloseFile()
{
	fclose(m_pStream);
	m_pStream = nullptr;
}