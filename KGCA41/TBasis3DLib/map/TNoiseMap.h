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
	float			m_fOctaves;		// ��Ÿ�� ����. 	
	float			m_fHurstIndex;	// �㽺Ʈ �ε���(�� ��Ÿ�꿡 ���� ������ �󸶳� ���� �� �ִ����� ����)
	float			m_fLacunarity;	// ���ؼ� ( ���ļ��� ��ȭ��	)
	
	float			m_fPersistence; // ���Ӽ�( ������ ��ȭ�� )	
	//float			m_fGain;		// ���Ӽ�( ������ ��ȭ�� ): 0�� ������ �ϸ��� ���� 1�� ������ ����� ������ ��������
	
	double*			m_pExponentArray;// ��������Ʈ(����)�迭

	float			m_fStepSizeAcrossRow;
	float			m_fStepSizeDownCol;	
public:
	float			PerlinNoise( float vertex[], double fPersistence, double fOctaves);
	// ������ ���� �(FBM:Fractional Brownian Motion)
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
