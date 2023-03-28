#include "TTimer.h"
namespace TBASIS {
float g_fSecPerFrame = 0.0f;
float g_fDurationTime = 0.0f;
}

bool TTimer::Init()
{
	QueryPerformanceCounter( &m_Frame );
	
	// 고해상도 타이머 지원여부 판단
	if( m_Frame.QuadPart == 0 )
	{
		return false;
	}

	m_FPS				= m_Frame;
	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0.0f;		

	Start();
	return true;
}
bool TTimer::Frame()
{
	// 1프레임의 시간 체크 
	// 현재 시간을 찾는다. 
	QueryPerformanceCounter( &m_Current );		
	g_fSecPerFrame = m_fSecPerFrame =  static_cast<float>( m_Current.QuadPart-m_Frame.QuadPart ) / static_cast<float>(m_Frequency.QuadPart);	
	g_fDurationTime = m_fDurationTime += m_fSecPerFrame;
	// 1초당 프레임 체크 	( 0 보다 크다는 것은 정수이기 때문에 1 이 된다. )
	if( ( ( m_Current.QuadPart-m_FPS.QuadPart ) / m_Frequency.QuadPart)  >= 1 )
	{		
		m_iFramePerSecond	= m_iFPSElapse;
		m_iFPSElapse		= 0;
		m_FPS				= m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;

	return true;
}

int	TTimer::GetFPS() 
{
	return m_iFramePerSecond;
}
float TTimer::GetSPF()
{
	return m_fSecPerFrame;
}

bool TTimer::Render()
{
	return true;
}
bool TTimer::Release()
{	
	return true;
}

void TTimer::Reset()
{
   m_bStarted = false;
   m_fEventTime = 0.0f;
   memset( &m_Start, 0, sizeof( LARGE_INTEGER ) );
   memset( &m_Elapse, 0, sizeof( LARGE_INTEGER ) );
}

void TTimer::Start()
{
   m_bStarted	= true;
   m_fEventTime = 0.0f;
   QueryPerformanceCounter((LARGE_INTEGER *)&m_Start);
}

void TTimer::Stop()
{
   GetElapsedTime();
   m_bStarted = false;
}


bool TTimer::IsStarted() const
{
   return m_bStarted;
}


float TTimer::GetElapsedTime()
{   
   if( m_bStarted )
   {
      QueryPerformanceCounter((LARGE_INTEGER *)&m_Elapse);
      m_fEventTime = static_cast<float>(m_Elapse.QuadPart - m_Start.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
   }
   return m_fEventTime;
}

TTimer::TTimer() 
{   
    m_bStarted			=  false;   	
   	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0.0f;	
	m_fEventTime		= 0.0f;
	m_fDurationTime = 0.0f;
   	// 시스템의 주파수 변동폭을 얻어 온다. 이는 시간의 표준을 잡기 위해서 사용한다.
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);
}

TTimer::~TTimer()
{
}