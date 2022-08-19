#include "TSpacePartition.h"
void    TSpacePartition::Create(TVector2D vPos, TVector2D vSize)
{

}
void    TSpacePartition::Create(TVector vPos, TVector vSize)
{

}
void   TSpacePartition::DynamicObjectReset()
{
}
// 1번 : 완전히 포함하는 노드에 추가하자.
// 2번 : 걸쳐만 있어도 노드에 추가하자.
void    TSpacePartition::AddStaticObject(TBaseObject* pObj)
{ 
}
void    TSpacePartition::AddDynamicObject(TBaseObject* pObj)
{  
}

TNode* TSpacePartition::FindNode(TNode* pNode, TBaseObject* pObj)
{    
    return nullptr;
}
std::vector<TBaseObject*> TSpacePartition::CollisionQuery(TBaseObject* pObj)
{
    std::vector<TBaseObject*> list;
    return list;
};

TSpacePartition::TSpacePartition()
{

}
TSpacePartition::~TSpacePartition()
{
}