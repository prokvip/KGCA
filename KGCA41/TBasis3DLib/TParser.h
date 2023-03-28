#pragma once
#include "TBasisStd.h"

enum SET_DATA_TYPES
{
	// Gameplay semantics
	NULL_DATA = 0,
	BOOL_DATA,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VERTEX_DATA,
	VECTOR_DATA,
	INDEX_DATA,
	FACE_DATA,
	MATRIX_DATA,
};

class TParser
{
public:
	FILE*	m_pStream;
	TCHAR	m_pBuffer[256];
	TCHAR	m_pString[256];
public:
	BOOL	ComparisonData(const TCHAR* pDestString, const TCHAR* pSrcString = NULL);
	bool	ErrorCloseStream(const TCHAR* pMsg);
	BOOL	OpenStream(const TCHAR* pFileName);
	BOOL    CloseStream() { fclose(m_pStream); return TRUE; };
	BOOL	GetData(VOID* pData, INT DataType);
	// 전체파일에서 해당 문자열을 찾을때 까지 검색하고 파일포인터를 다음라인으로 옮긴다.
	BOOL	GetDataFromFileLoop(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// 다음라인을 대상으로 문장열을 찾고 다음라인으로 파일포인터를 옮긴다.
	BOOL	GetDataFromFileNext(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// 원하는 문자열을 찾고 찾은 문자열 시작위치로 옮긴다
	BOOL	GetDataFromFileString(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	// 다음라인을 대상으로 해당문자열을 찾고 검색을 실폐하면 이전 라인으로 파일포인터를 옮긴다.
	BOOL	GetDataFromFile(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	BOOL    GetNodeName(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
public:
	TParser(void);
	virtual ~TParser(void);
};
