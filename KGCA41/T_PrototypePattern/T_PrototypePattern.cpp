// TCompositePattern.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
class TGraphicObject
{
public:
    TGraphicObject()
    {
    }
    TGraphicObject(const TGraphicObject& a)
    {
        for (int i = 0; i < 100; i++)
        {
            iData[i] = this->iData[i];
        }
    }
public:
    int*   pData;
    int    iData[100];
    virtual TGraphicObject* Clone() = 0;
};
class TTriangle : public TGraphicObject
{
public:
    TTriangle()
    {
    }
    /*TTriangle(const TTriangle& a)
    {
        for (int i = 0; i < 100; i++)
        {
            iData[i] = a.iData[i];
        }
        pData = a.pData;
        pData = new int[100];
        for (int i = 0; i < 100; i++)
        {
            pData[i] = a.pData[i];
        }
    }*/
    virtual TGraphicObject* NewObject()
    {
        return new TTriangle;
    }
    virtual TGraphicObject* Clone()
    {
        return new TTriangle(*this);
    }
};
int main()
{
    TTriangle* pTriObj = new TTriangle;
    for (int i = 0; i < 100; i++)
    {
        pTriObj->iData[i] = rand();
    }
    TTriangle t = *pTriObj;
    TGraphicObject* pObjA = new TTriangle;
    TGraphicObject* pObjB = pTriObj->Clone();
    /*for (int i = 0; i < 100; i++)
    {
        pTriObj->iData[i] = rand();
    }*/

    std::cout << "Hello World!\n";
}
