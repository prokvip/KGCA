#pragma once
#include "TMap.h"
#include "TNoise.h"

class TNoiseMap : public TMap
{
public:
	enum {
		PERLIN_NOISE = 0,
		FBM,			 
		MAX_COUNT,
	};
	int				m_iNoiseType;
	TNoise			m_Noise;	
	float			m_fStepSize;
	//float			m_fOffset;		//  
	float			m_fOctaves;		// 옥타브 갯수. 	
	float			m_fHurstIndex;	// 허스트 인덱스(각 옥타브에 대해 진폭이 얼마나 변할 수 있는지를 결정)
	float			m_fLacunarity;	// 공극성 ( 주파수의 변화량	)
	
	float			m_fPersistence; // 지속성( 진폭의 변화량 )	
	//float			m_fGain;		// 지속성( 진폭의 변화량 ): 0에 가까우면 완만한 지형 1에 가까우면 결렬한 노이즈 지형생성
	
	double*			m_pExponentArray;// 엑스포넌트(지수)배열

	float			m_fStepSizeAcrossRow;
	float			m_fStepSizeDownCol;	
public:
	float			PerlinNoise( float vertex[], double fPersistence, double fOctaves);
	// 프랙털 브라운 운동(FBM:Fractional Brownian Motion)
	double			fBM( float point[], double fHurstIndex, double fLacunarity, double fOctaves );
	void			Set( float fStepSize= 10.0f, float fHurstIndex=2.0f, float fLacunarity=0.651f, float fOctaves=5.815f );
	bool			CreateVertexData();
	void			ReSet();
	void			SetNoiseType( int iType ) 
	{ 
		m_iNoiseType = iType; 		
		ReSet();
		CreateVertexData();
		ReLoadVBuffer();
	}
	void			SetHurstIndex( bool bAdd=true ) 
	{
		if( bAdd )	m_fHurstIndex	+= 0.2f; 
		else		m_fHurstIndex	-= 0.2f; 
		CreateVertexData();
		ReLoadVBuffer();
	};
	void			SetLacunarity( bool bAdd=true ) 
	{
		if( bAdd )	m_fLacunarity	+= 0.02f; 
		else		m_fLacunarity	-= 0.02f; 
		CreateVertexData();
		ReLoadVBuffer();
	};
	void			SetOctaves( bool bAdd=true ) 
	{
		if( bAdd )	m_fOctaves	+= 0.2f; 
		else		m_fOctaves	-= 0.2f; 	
		CreateVertexData();
		ReLoadVBuffer();
	};
	void			SetPersistence( bool bAdd=true ) 
	{
		if( bAdd )	m_fPersistence	+= 0.1f; 
		else		m_fPersistence	-= 0.1f; 	
		CreateVertexData();
		ReLoadVBuffer();
	};

public:
	TNoiseMap(void);
	virtual ~TNoiseMap(void);
};
