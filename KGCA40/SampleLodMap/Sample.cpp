#include "Sample.h"
// 1)리프노드 랜더링
//   - 공유 정점버퍼(맵), 리프노드 당 인덱스 버퍼 사용
// 2)리프노드 랜더링
//   - 리프노드 당 정점버퍼, 공유되는 인덱스 버퍼 사용
//   = LOD -> 패치단위로 16개의 정점 인덱스 버퍼가 있어야 된다. 
//  LOD
// 1) 리프노드에 패치 몇개야 ? 
//   - PatchList[][16] 인덱스버퍼가 있다.
bool Sample::Init()
{
    m_Texture.LoadTexture(L"../../data/bitmap1.BMP");
    TMapInfo info{
            16 + 1, 
            16 + 1, 0,0, 0,
            10.0f
    };
    if (!m_Map.Load(info, L"../../data/shader/DefaultShader.hlsl", 
        L"../../data/shader/DefaultShader.hlsl"))
    {
        return false;
    }
    m_Quadtree.LoadObject(L"../../data/script/StaticLod.txt");
    m_Quadtree.Build(&m_Map);    
   
    return true;
}
bool Sample::Frame()
{
     m_Map.Frame();
    return true;
}

bool Sample::Render()
{   
    m_Map.SetMatrix(
        nullptr,
        &m_Camera.m_matView,
        &m_Camera.m_matProj);
    //m_Map.Render(m_pImmediateContext);

    m_pImmediateContext->PSSetSamplers(0, 1, &m_Texture.m_pSampler);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_Texture.m_pTextureSRV);
    m_Quadtree.Render(m_pImmediateContext, m_Camera.m_vCameraPos);
    return false;
}

bool Sample::Release()
{
    m_Texture.Release();
    m_Map.Release();
    m_Quadtree.Release();
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Release();
    }*/
   
    return false;
}

Sample::Sample()
{
}
Sample::~Sample()
{
}

TBASIS_RUN(SampleLibrary)