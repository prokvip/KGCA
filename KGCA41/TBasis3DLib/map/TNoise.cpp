#include "TNoise.h"
#include <time.h>

float TNoise::bias(float a, float b)
{
	return (float)pow((float)a, (float)(log(b) / log(0.5)));
}
// 진폭의 변화량
float TNoise::Gain(float a, float b)
{
	float p = (float)log(1.0f - b) / (float)log(0.5f);

	if (a < 0.001f)		return 0.0f;
	else if (a > 0.999f)return 1.0f;

	if (a < 0.5f)		return (float)pow(2 * a, p) / 2;
	else				return 1.0f - (float)pow((float)(2.0f * (1.0f - a)), (float)p) / 2.0f;
}
float TNoise::Noise(float vec[], int len)
{
	switch (len) 
	{
		case 0:
			return 0.;
		case 1:
			return Noise1(vec[0]);
		case 2:
			return Noise2(vec);
		default:
			return Noise3(vec);
	}
}
// 타뷸런스는 진폭의 변화폭을 결정한다..
float TNoise::Turbulence(float *v, float freq)
{
	float t, vec[3];
	for (t = 0. ; freq >= 1. ; freq /= 2) 
	{
		vec[0] = freq * v[0];
		vec[1] = freq * v[1];
		vec[2] = freq * v[2];
		t += (float)fabs(Noise3(vec)) / freq;
	}
	return t;
}
void	TNoise::Setup( float fValue, int& b0, int& b1, float& r0, float& r1 )
{
	b0		= ((int)fValue) & BM;
	b1		= (b0+1) & BM;
	r0		= fValue - (int)fValue;
	r1		= r0 - 1.0f;
}

// 일차원 노이즈 생성
float TNoise::Noise1(float arg)
{
	int bx0, bx1;
	float rx0, rx1, sx, u, v;

	if (m_IsStart) 
	{
		m_IsStart = 0;
		Init();
	}

	Setup( arg, bx0, bx1, rx0, rx1 );
	
	sx = s_curve(rx0);

	u = rx0 * m_vRandomData[ m_iRandomIndex[ bx0 ] ][0];
	v = rx1 * m_vRandomData[ m_iRandomIndex[ bx1 ] ][0];

	return lerp(sx, u, v);
}
// 2차원 노이즈 생성
float TNoise::Noise2(float vec[2])
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, u, v;
	register int i, j;

	if (m_IsStart) 
	{
		m_IsStart = 0;
		Init();
	}

	Setup( vec[0], bx0, bx1, rx0, rx1 );
	Setup( vec[1], by0, by1, ry0, ry1 );

	i = m_iRandomIndex[ bx0 ];
	j = m_iRandomIndex[ bx1 ];

	b00 = m_iRandomIndex[ i + by0 ];
	b10 = m_iRandomIndex[ j + by0 ];
	b01 = m_iRandomIndex[ i + by1 ];
	b11 = m_iRandomIndex[ j + by1 ];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = m_vRandomData[ b00 ] ; u = at2(rx0,ry0);
	q = m_vRandomData[ b10 ] ; v = at2(rx1,ry0);
	a = lerp(sx, u, v);

	q = m_vRandomData[ b01 ] ; u = at2(rx0,ry1);
	q = m_vRandomData[ b11 ] ; v = at2(rx1,ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);
}

// 3차원 노이즈 생성
float TNoise::Noise3(float vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register int i, j;

	if (m_IsStart) 
	{
		m_IsStart = 0;
		Init();
	}

	Setup( vec[0], bx0, bx1, rx0, rx1 );
	Setup( vec[1], by0, by1, ry0, ry1 );
	Setup( vec[2], bz0, bz1, rz0, rz1 );	

	i = m_iRandomIndex[ bx0 ];
	j = m_iRandomIndex[ bx1 ];

	b00 = m_iRandomIndex[ i + by0 ];
	b10 = m_iRandomIndex[ j + by0 ];
	b01 = m_iRandomIndex[ i + by1 ];
	b11 = m_iRandomIndex[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = m_vRandomData[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = m_vRandomData[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = m_vRandomData[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = m_vRandomData[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = m_vRandomData[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = m_vRandomData[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = m_vRandomData[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = m_vRandomData[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

void TNoise::Normalize2(float v[2])
{
	float s;
	s = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
}

void TNoise::Normalize3(float v[3])
{
	float s;
	s = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}

void TNoise::Init(void)
{
	//srand((unsigned)time(NULL)); 
	int i, j, k;
	for(i = 0 ; i < B ; i++) 
	{
		m_iRandomIndex[i] = i;
		for (j = 0 ; j < 3 ; j++)
		{
			m_vRandomData[i][j] = (float)((rand() % (B + B)) - B) / B;
		}
		Normalize3(m_vRandomData[i]);
	}

	while(--i) 
	{
		k = m_iRandomIndex[i];
		m_iRandomIndex[i] = m_iRandomIndex[rand() % B];
		m_iRandomIndex[j] = k;
	}

	for(i = 0 ; i < B  ; i++) 
	{
		m_iRandomIndex[B + i] = m_iRandomIndex[i];
		for (j = 0 ; j < 3 ; j++)
		{
			m_vRandomData[B + i][j] = m_vRandomData[i][j];
		}
	}
}
float TNoise::Perlin_Noise_2D(float vec[2])
{
	int		terms    = m_iOctaves;
	float	freq   = m_fFrequency;
	float	result = 0.0f;
	float	amp = m_fAmplitude;  
	vec[0]*=m_fFrequency;

	vec[1]*=m_fFrequency;	for( int i=0; i<terms; i++ )
	{
		result += Noise2(vec)*amp;
		vec[0] *= 2.0f;
		vec[1] *= 2.0f;
		amp*=0.5f;
	}
	return result;
}
void TNoise::Set(int octaves,float freq,float amp,int seed)
{
	m_iOctaves = octaves;
	m_fFrequency = freq;
	m_fAmplitude = amp;
	m_iSeed = seed;
	m_IsStart = true;
}

TNoise::TNoise(void)
{
	m_IsStart = 1;	
}

TNoise::~TNoise(void)
{
}
