// T_FactoryMethodPattern.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
class TBaseObject {
public:
    TBaseObject() {};
    virtual ~TBaseObject() {}
};
class TObject3D : public TBaseObject {
public:
    TObject3D() {};
};
class TObject2D : public TBaseObject {
public:
    TObject2D() {};
};

class TSpaceDivision{
public:
    std::vector<TBaseObject*> m_list;
public:
    virtual TBaseObject* NewObject() = 0;
    virtual void  add(TBaseObject* obj) 
    {
        m_list.push_back(obj);
    }
};
class TQuadtree : public TSpaceDivision
{
public:
    TBaseObject* NewObject()
    {
        return new TObject2D;
    }    
};
class TOctree : public TSpaceDivision
{    
public:
    TBaseObject* NewObject()
    {
        return new TObject3D;
    }
};
int main()
{
    TSpaceDivision* sd = new TQuadtree;
    //TSpaceDivision* sd = new TOctree; 
    TBaseObject* obj = sd->NewObject();
    //sd->add(sd->NewObject());
    sd->add(obj);   
    std::cout << "Hello World!\n";
}
