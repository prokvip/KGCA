#include "TQuadtree.h"

int main()
{    
    float fRadian = DegreeToRadian(180);
    float fDegree = RadianToDegree(fRadian);
    TVector2D va(800,0);
    TVector2D vb(800, 800);
    //TVector2D vaa = va.Identity();
    //TVector2D vbb = vb.Identity();    
    float fAngle = va.Angle(vb);
    TVector2D vc = va + vb;
    TVector2D vd = va - vb;
    float  fA = va.Length();
    float  fB = vb.Length();

    TVector2D vf(2, 2);
    float  fF = vf.Length();

    TVector2D vIdentityA = va.Identity();
    TVector2D vIdentityB = vb.Identity();

    float fLengthA = vIdentityA.Length();
    float fLengthB = vIdentityB.Length();

    TVector2D vaA = vIdentityA * fA;
    TVector2D vaB = vIdentityB * fB;

    vaA.Normalized();
    vaB.Normalized();
    if (va == vb)
    {

    }
    //std::vector<int> list1 = { 4,5,64, };
    //std::vector<int> list2;
    //list2 = list1;

    TObject player;
    player.SetPosition(0, 100, 10, 10);
    TQuadtree quadtree;
    quadtree.Create(100.0f, 100.0f);
    float fDistance = 0.0f;

    std::map<int, TObject*> objList;
    std::map<int, TObject*> AllobjList;
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        AllobjList.insert(std::make_pair(iObj, pObj));
        quadtree.AddStaticObject(pObj);        
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        objList.insert(std::make_pair(iObj, pObj));
        AllobjList.insert(std::make_pair(iObj, pObj));
        quadtree.AddDynamicObject(pObj);
    }
 /*   TObject* pObj = new TObject;
    pObj->SetPosition(100, 0, 20, 20);
    quadtree.AddObject(pObj);

    if (TCollision::CircleToCircle(
        player.m_Circle,
        pObj->m_Circle))
    {

    }*/

    //player.m_vDirection.x = 1.0f;
    //player.m_vDirection.y = 0.0f;

    while (1)
    {   
        quadtree.DynamicObjectReset(quadtree.m_pRootNode);
        for (auto obj : objList)
        {
            TObject* pObject = obj.second;
            pObject->Frame();
            quadtree.AddDynamicObject(pObject);
        }
        TVector2D vPos(player.m_rt.x1, player.m_rt.y1);
        // 벡터의 직전의 방정식
       //  결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간) 
        TVector2D vTmp = player.m_vDirection * 1.0f;
        vPos = vPos + vTmp;
        player.m_rt.x1 = vPos.x;
        player.m_rt.y1 = vPos.y;

        if (vPos.x > 100.0f)
        {
            player.m_rt.x1 = 100.0f;
            player.m_vDirection.x *= -1.0f;
        }
        if (vPos.x < 0.0f)
        {
            player.m_rt.x1 = 0.0f;
            player.m_vDirection.x *= -1.0f;
        }
        if (vPos.y > 100.0f)
        {
            player.m_rt.y1 = 100.0f;
            player.m_vDirection.y *= -1.0f;
        }
        if (vPos.y < 0.0f)
        {
            player.m_rt.y1 = 0.0f;
            player.m_vDirection.y *= -1.0f;
        }
        player.SetPosition(player.m_rt.x1, player.m_rt.y1, 30, 30);


        std::vector<TObject*> list = quadtree.Collision(&player);
        std::cout << "player:"
            << player.m_rt.x1 << "," << player.m_rt.y1 << ","
            << player.m_rt.x2 << "," << player.m_rt.y2
            << std::endl;
        if (!list.empty())
        {
            for (int iObj = 0; iObj < list.size(); iObj++)
            {
                std::cout << "object:"
                    << list[iObj]->m_rt.x1 << "," << list[iObj]->m_rt.y1 << ","
                    << list[iObj]->m_rt.x2 << "," << list[iObj]->m_rt.y2
                    << std::endl;
            }
        }             
        
        Sleep(100);
        system("cls");
    }

    for (auto object : AllobjList)
    {
        delete object.second;
    }
    AllobjList.clear();
    objList.clear();
    //화면좌표계(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
    //0,0 -> x       50,0         100(x), 0(y)        
    //|    0                  1
    //y 0,50         50,50        100,50   
    //     2                  3 
    //0,100          50,100       100,100 
    std::cout << "Hello World!\n";
}
