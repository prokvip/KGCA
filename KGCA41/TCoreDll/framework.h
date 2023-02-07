// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#ifdef TCOREDLL_EXPORTS
	#define TCOREDLL_API __declspec(dllexport)
#else
	#define TCOREDLL_API __declspec(dllimport)
#endif