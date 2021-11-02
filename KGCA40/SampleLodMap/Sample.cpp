#include "Sample.h"
// 1)������� ������
//   - ���� ��������(��), ������� �� �ε��� ���� ���
// 2)������� ������
//   - ������� �� ��������, �����Ǵ� �ε��� ���� ���
//   = LOD -> ��ġ������ 16���� ���� �ε��� ���۰� �־�� �ȴ�. 
//  LOD
// 1) ������忡 ��ġ ��� ? 
//   - PatchList[][16] �ε������۰� �ִ�.
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