// TCompositePattern.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <map>
class TGraphicObject
{
public:
    virtual TGraphicObject* Clone() = 0;
};
class TTriangle : public TGraphicObject
{
public:
    int*    pDataTTriangle;
    int    iDataTTriangle[100];
public:
    virtual TGraphicObject* Clone()
    {
        return new TTriangle(*this);
    }
};
class TRectangle : public TGraphicObject
{
public:
    int*   pDataTRectangle;
    int    iDataTRectangle[100];
public:
    virtual TGraphicObject* NewObject()
    {
        return new TRectangle;
    }
    virtual TGraphicObject* Clone()
    {
        return new TRectangle(*this);
    }
};
class TComposite : public TGraphicObject
{
public:
    std::vector<TGraphicObject*> component;
public:
    TComposite()
    {
    }
    virtual TGraphicObject* Clone()
    {
        TComposite* thisNew =  new TComposite();
        for (TGraphicObject* data : component)
        {
            thisNew->component.push_back(data->Clone());
        }
        return thisNew;
    }
};
template<class T> 
class TSingleton
{
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }
};
class TPaletteSingleton : public TSingleton<TPaletteSingleton>
{
    friend class TSingleton<TPaletteSingleton>;
public:
    std::map<int, TGraphicObject*> m_list;
    void  RegisterNewGraphic(TGraphicObject* pNewGraphic)
    {
        m_list.insert(std::make_pair(m_list.size(), pNewGraphic));
    }
private:
    TPaletteSingleton() {
        TGraphicObject* a = new TTriangle;
        TGraphicObject* b = new TRectangle;
        m_list.insert(std::make_pair(0, a));
        m_list.insert(std::make_pair(1, b));
    };
public:
    ~TPaletteSingleton() {
        for (auto data : m_list)
        {
            delete data.second;
        }
        m_list.clear();
    };
};
#define g_Palette TPaletteSingleton::GetInstance()

class TPalette
{
public:
    std::map<int, TGraphicObject*> m_list;
    static TPalette* g_pPalette;
    static TPalette* GetInstance()
    {
        if (g_pPalette == nullptr)
        {
            g_pPalette = new TPalette;
        }
        return g_pPalette;
    }
private:
    TPalette() {};
public:
    ~TPalette() {
        for (auto data : m_list)
        {
            delete data.second;
        }
        m_list.clear();
    };
};
TPalette* TPalette::g_pPalette = nullptr;

int main()
{
    TPaletteSingleton paletteG = TPaletteSingleton::GetInstance();
    TPaletteSingleton paletteH = g_Palette;

    TComposite* pNewModel0 = new TComposite;
    pNewModel0->component.push_back(g_Palette.m_list[0]->Clone());
    pNewModel0->component.push_back(g_Palette.m_list[1]->Clone()); 
    g_Palette.RegisterNewGraphic(pNewModel0);

    TComposite* pNewModel1 = new TComposite;
    pNewModel1->component.push_back(g_Palette.m_list[0]->Clone());
    pNewModel1->component.push_back(g_Palette.m_list[1]->Clone());
    pNewModel1->component.push_back(g_Palette.m_list[2]->Clone());
    pNewModel1->component.push_back(g_Palette.m_list[2]->Clone());
    g_Palette.RegisterNewGraphic(pNewModel1);


    TComposite* pNewModel2 = new TComposite;
    pNewModel2->component.push_back(g_Palette.m_list[0]->Clone());
    pNewModel2->component.push_back(g_Palette.m_list[1]->Clone());
    pNewModel2->component.push_back(g_Palette.m_list[2]->Clone());
    pNewModel2->component.push_back(g_Palette.m_list[3]->Clone());
    g_Palette.RegisterNewGraphic(pNewModel2);

    TComposite* pNewModel3 = new TComposite;
    TGraphicObject* pA = g_Palette.m_list[4]->Clone();    
    pNewModel3->component.push_back(pA);
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());
    pNewModel3->component.push_back(g_Palette.m_list[4]->Clone());

    g_Palette.RegisterNewGraphic(pNewModel3);

    
    TPalette* paletteA = TPalette::GetInstance();
    TPalette* paletteB = TPalette::GetInstance();
    TPalette* paletteD = TPalette::GetInstance();
    TPalette* paletteE = TPalette::GetInstance();

}
//int main()
//{
//    TTriangle* pTriObj = new TTriangle;
//    for (int i = 0; i < 100; i++)
//    {
//        pTriObj->iDataTTriangle[i] = rand();
//    }
//    TRectangle* pRectObj = new TRectangle;
//    for (int i = 0; i < 100; i++)
//    {
//        pRectObj->iDataTRectangle[i] = rand();
//    }
//
//    TGraphicObject* pObjA = pTriObj->Clone();
//    TGraphicObject* pObjB = pRectObj->Clone();
//
//    TComposite*     pObjD = new TComposite;
//    pObjD->component.push_back(pObjA->Clone());
//    pObjD->component.push_back(pObjB->Clone());
//
//    TGraphicObject*  pObjE = pObjD->Clone();
//    std::cout << "Hello World!\n";
//}
