#include "Sample.h"
void  Sample::CreateBlendState()
{
    // alpha blending
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    // 알파블랜딩 공식 - 소스(float4(0,1,0,0.5f)), 대상(1,0,0,1)
    //finalcolor(RGB) = SrcColor* SrcBlend   + DestColor*DestBlend
    //           = Scrcolor* alphaValue + DestColor * (1.0f-alphaValue)
    //           = 0,1,0 * 0.5f + 1,0,0 * (1.0f-0.5f)
    // //        만약 t= 0.0f; 결과 => 배경만 나온다.
    // //        만약 t= 1.0f; 결과 => 소스만 나온다.
    //           만약 t= 0.5f; 결과 - >소스컬러*0.5 + 배경컬러*0.5f
    //           = 0,1,0 * t + 1,0,0 * (1.0f-t)
    //           =  r*t,b*t,b*t + r*(1.0f-t),b*(1.0f-t),b*(1.0f-t)

    // (A)
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);
}
bool  Sample::Init() 
{   
    CreateBlendState();
    m_texMgr.Set(m_pDevice, m_pImmediateContext);
    m_shaderMgr.Set(m_pDevice, m_pImmediateContext);

    std::wstring texname[] = { 
        L"../../res/kgcabk.bmp", // path=../../res/, name = kgcabk.bmp
        L"../../res/ade4.dds" ,
        L"../../res/mapcontrol.png",
        L"../../res/103.tga"
    };

    srand(time(NULL));
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TPlaneObj;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(TVector3(randstep(-1.0f, +1.0f),
                     randstep(-1.0f, +1.0f), 
                     0));
        pObj->m_vScale.x = randstep(0.1f, 0.2f);
        pObj->m_vScale.y = randstep(0.1f, 0.2f); 
        pObj->m_vScale.z = 1.0f; 
        pObj->Create( m_texMgr, texname[iObj % 4],
                      m_shaderMgr, L"Plane.hlsl");
        m_ObjList.push_back(pObj);
    }
    return true; 
}
bool  Sample::Frame() 
{ 
    for (auto obj : m_ObjList)
    {
        //std::vector<PT_Vertex> vList;
        //vList.resize(obj->m_VertexList.size());
        //TMatrix matWorld,matScale, matRotation, matTranslate;
        //matScale.Sclae(0.5f, 0.5f, 0.5f);
        //matRotation.ZRotate( DegreeToRadian(g_fGameTimer*100.0f) );
        //matTranslate.Translation(cosf(g_fGameTimer), 0, 0);
        //matWorld = matScale * matRotation * matTranslate;
        //for (int iVertex = 0; iVertex < obj->m_VertexList.size(); iVertex++)
        //{
        //    vList[iVertex].t = obj->m_VertexList[iVertex].t;
        //    vList[iVertex].p = obj->m_VertexList[iVertex].p * matWorld;
        //}
        //// gpu 처리
        //m_pImmediateContext->UpdateSubresource(
        //    obj->m_pVertexBuffer,
        //       0, nullptr, &vList.at(0), 0, 0);
        //obj->SetPos(TVector3(cosf(g_fGameTimer), 0, 0));
        obj->Frame();
    }
    return true; 
}
bool  Sample::Render() 
{    
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    for (auto obj : m_ObjList)
    {
        
        obj->SetMatrix(nullptr, nullptr, nullptr);
        obj->Render();
    }
    return true; 
}
bool  Sample::Release()
{   
    for (auto obj : m_ObjList)
    {
        obj->Release();
        delete obj;
    }
    m_ObjList.clear();  
    m_AlphaBlend->Release();
    return true; 
}

TGAME(L"kgca", 800, 600)