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
// 1�� : ������ �����ϴ� ��忡 �߰�����.
// 2�� : ���ĸ� �־ ��忡 �߰�����.
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