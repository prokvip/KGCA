#pragma once
#include <stdlib.h>
#include <math.h>

const int	B	= 0x100;	// 256
const int	BM  = 0xff;		// 255
//const int	N	= 0x1000;	// 4096
//const int   NP	= 12;		/* 2^N */
//const int	NM  = 0xfff;	// 4095

#define s_curve(t)		( t * t * (3.0f - 2.0f * t) )
//#define s_curve(t)		( (6*t*t*t*t*t*t)-(15*t*t*t*t)+(10*t*t*t) )
#define lerp(t, a, b)	( a + t * (b - a) )

/////////////////////////////////////////////////////
class TNoise
{
public:
	int		m_iRandomIndex[B + B ];
	float	m_vRandomData[B + B ][3];
	int		m_IsStart;	
  
	int		m_iOctaves;
	float	m_fFrequency;
	float	m_fAmplitude;
	int		m_iSeed;

public:
	void	Set(int octaves,float freq,float amp,int seed);
	void	Setup( float fValue, int& b0, int& b1, float& r0, float& r1 );
	float	Perlin_Noise_2D(float vec[2]); 

	float	bias(float a, float b);	
	float	Gain(float a, float b);// ������ ��ȭ��	
	float	Turbulence(float *v, float freq);// Ÿ�深���� ������ ��ȭ���� �����Ѵ�..	
	
	void	Init(void);				// 3���� ������ �����ϱ� ���� �ʱ�ȭ	
	float	Noise(float vec[], int len);	
	float	Noise1(float arg);		// ������ ������ ����	
	float	Noise2(float vec[2]);	// 2���� ������ ����
	float	Noise3(float vec[3]);	// 3���� ������ ����	

	void	Normalize2(float v[2]);	// 2���� ���� ����ȭ
	void	Normalize3(float v[3]);	// 3���� ���� ����ȭ

public:
	TNoise(void);
	~TNoise(void);
};
