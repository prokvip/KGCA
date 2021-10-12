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
    TMapInfo info{
            16 + 1, 
            16 + 1, 0,0, 0,
            10.0f
    };
    if (!m_Map.Load(info, L"../../data/shader/VSMap.txt", 
        L"../../data/shader/PSMap.txt"))
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
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Frame();
    }*/   
    return true;
}

bool Sample::Render()
{   
    m_Map.SetMatrix(
        nullptr,
        &m_Camera.m_matView,
        &m_Camera.m_matProj);
    //m_Map.Render(m_pImmediateContext);
    m_Quadtree.Render(m_pImmediateContext, m_Camera.m_vCameraPos);
    return false;
}

bool Sample::Release()
{
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