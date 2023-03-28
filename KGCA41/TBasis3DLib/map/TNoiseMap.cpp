#include "TNoiseMap.h"

void TNoiseMap::Set( float fStepSize, float fHurstIndex, float fLacunarity, float fOctaves )
{
	m_fHurstIndex	= fHurstIndex;
	m_fLacunarity	= fLacunarity;
	m_fOctaves		= fOctaves;

	m_fStepSizeAcrossRow	= fStepSize / float( m_iNumRows-1 );
	m_fStepSizeDownCol		= fStepSize / float( m_iNumCols-1 );

	// 노이즈 배열의 최대 갯수안으로 StepSize 제한
	if( B <= ( m_fStepSizeDownCol * m_iNumCols ) ) 
	{
		m_fStepSizeDownCol	= B / float( m_iNumCols-1 );		
	}
	if( B <= ( m_fStepSizeAcrossRow * m_iNumRows ) ) 
	{
		m_fStepSizeAcrossRow	= B / float( m_iNumRows-1 );		
	}

	SAFE_NEW_ARRAY( m_pExponentArray, double, (size_t)m_fOctaves+1 );
	float frequency = 1.0;
	for( int i=0; i<= m_fOctaves; i++)
	{
		m_pExponentArray[i] = pow( frequency, -m_fHurstIndex );
		frequency *= m_fLacunarity;
	}
}

bool TNoiseMap::CreateVertexData()
{
	if( m_VertexList.size() <= 0 )
	{		
		m_VertexList.resize(m_iNumVertices );	// 높이맵배열 생성	
	}

	Set( m_fStepSize, m_fHurstIndex, m_fLacunarity, m_fOctaves );
	/*if( m_iNoiseType == PERLIN_NOISE)		Set( 5.0f, 0.5f, 0.651f, 5.815f );	
	if( m_iNoiseType == FBM )				Set( 20.0f, 2.0f, 0.601f, 5.815f );	*/

	int iHalfCols		= m_iNumCols / 2;
	int iHalfRows		= m_iNumRows / 2;
	float ftxOffsetU	= 1.0f / (m_iNumCols-1);
	float ftxOffsetV	= 1.0f / (m_iNumRows-1);

	float vertex[3], fNoiseValue;
	for( int iRow = 0; iRow < m_iNumRows; iRow++ )
	{
		for( int iCol = 0; iCol < m_iNumCols; iCol++ )
		{
			vertex[0] = m_fStepSizeDownCol * iCol;
			vertex[1] = m_fStepSizeAcrossRow * iRow;
			vertex[2] = 0.0f;

			int  iVertexIndex = iRow * m_iNumCols + iCol;			
			m_VertexList[iVertexIndex].p.x  = ( iCol-iHalfCols )		* m_fCellDistance;    
			m_VertexList[iVertexIndex].p.z  = -( ( iRow-iHalfRows )	* m_fCellDistance );	
			
			if( m_iNoiseType == PERLIN_NOISE)
			{
				fNoiseValue = (float)PerlinNoise(vertex, m_fPersistence, m_fOctaves);
				m_VertexList[iVertexIndex].p.y  = fNoiseValue * 25.0f;
				m_VertexList[iVertexIndex].c    = TVector4( ( fNoiseValue + 1 ) / 2.0f,
																( fNoiseValue + 1 ) / 2.0f,
																( fNoiseValue + 1 ) / 2.0f,
																1.0f );
			}
			if( m_iNoiseType == FBM)
			{
				fNoiseValue  = (float)fBM( vertex, m_fHurstIndex, m_fLacunarity, m_fOctaves );
				m_VertexList[iVertexIndex].p.y = fNoiseValue;
				m_VertexList[iVertexIndex].c    = TVector4( ( fNoiseValue + 75 ) / 150.0f,
																( fNoiseValue + 75 ) / 150.0f,
																( fNoiseValue + 75 ) / 150.0f,
																1.0f );
			}

			m_VertexList[iVertexIndex].n    = GetNormalOfVertex( iVertexIndex ); 

			m_VertexList[iVertexIndex].t.x  = ftxOffsetU * iCol; 
			m_VertexList[iVertexIndex].t.y  = ftxOffsetV * iRow;				
		}
	}
	return true;
}

double IntegerNoise(int n)
{
    n = (n >> 13) ^ n;
    int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return 1.0 - ((double)nn / 1073741824.0);
}
////펄린 브라운 모션
////                                       정정리스트 ,  H계수,        진폭                       옥타브
//double TNoiseMap::fBm( float vertex[], double H, double lacunarity, double octaves )
//{
//	double remainder;	
//	double value = 0.0;
//
//	for(int i=0; i< octaves; i++)
//	{
//		value		+= m_Noise.Noise3( vertex ) * m_pExponentArray[i];
//		vertex[0]	*= (float)lacunarity;
//		vertex[1]	*= (float)lacunarity;
//		vertex[2]	*= (float)lacunarity;
//	}
//
//	remainder = octaves - (int)octaves;
//	if (remainder)
//		value += remainder * m_Noise.Noise3( vertex ) * m_pExponentArray[(int)octaves];
//
//	return value;
//}
double TNoiseMap::fBM( float vertex[], double fHurstIndex, double fLacunarity, double fOctaves )
{
	//double fResult = m_Noise.Noise3( vertex );
	double fResult = m_Noise.Noise( vertex,3 );
	double Frequency=1.0f;
	double Amplitude = 1.0f;

	for( int i=1; i <  (int)fOctaves; i++ )
	{
		vertex[0]	*= (float)fLacunarity;
		vertex[1]	*= (float)fLacunarity;
		vertex[2]	*= (float)fLacunarity;
		Frequency	*= (float)fLacunarity;// 공극성( 주파수의 변화량 )
		Amplitude	= pow( Frequency, -fHurstIndex );
		//fResult	+= Amplitude * m_Noise.Noise3( vertex );
		fResult		+= Amplitude * m_Noise.Noise( vertex,3 );
	}	
	double remainder = fOctaves - (int)fOctaves;
	if(remainder)
	{
		//fResult	+= remainder * m_Noise.Noise3( vertex ) * Amplitude;
		fResult		+= remainder * m_Noise.Noise( vertex,3 ) * Amplitude;
	}
	return fResult;
}
float TNoiseMap::PerlinNoise( float vertex[], double fPersistence, double fOctaves )
{
	float fResult = 0.0f;
	float n = (int)fOctaves - 1.0f;

	float Frequency	=1.0f;
	float Amplitude = 1.0f;

	float fV[3];
	for( int i =0; i < (int)fOctaves;i++ )
	{
		Frequency = (float)pow( 2.0f, i );
		Amplitude = (float)pow( fPersistence, i );

		fV[0] = vertex[0] * Frequency;
		fV[1] = vertex[1] * Frequency;		
		fV[2] = vertex[2] * Frequency;
		fResult +=  Amplitude * m_Noise.Noise( fV,3 );
	}
	double remainder = fOctaves - (int)fOctaves;
	if(remainder)
	{
		//fResult	+= remainder * m_Noise.Noise3( vertex ) * Amplitude;
		fResult		+= (float)remainder * m_Noise.Noise( fV,3 ) * Amplitude;
	}
	return fResult;
}
void TNoiseMap::ReSet()
{
	if( m_iNoiseType == PERLIN_NOISE)
	{
		m_fHurstIndex	= 0.5f;
		m_fLacunarity	= 0.651f;
		m_fOctaves		= 5.815f;
		m_fPersistence	= 0.5f; 
		m_fStepSize		= 5.0f;
	}		
	if( m_iNoiseType == FBM )	
	{
		m_fHurstIndex	= 2.0f;
		m_fLacunarity	= 0.651f;
		m_fOctaves		= 5.815f;
		m_fPersistence	= 0.5f; 
		m_fStepSize		= 20.0f;
	}
}
TNoiseMap::TNoiseMap(void)
{
	m_iNoiseType	= 1;
	m_pExponentArray= NULL;	
	ReSet();	
}


TNoiseMap::~TNoiseMap(void)
{
	SAFE_DELETE_ARRAY( m_pExponentArray );
}
