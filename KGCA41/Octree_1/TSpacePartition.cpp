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
void    TSpacePartition::AddStaticObject(TObject* pObj)
{ 
}
void    TSpacePartition::AddDynamicObject(TObject* pObj)
{  
}
void    TSpacePartition::AddStaticObject2D(TObject2D* pObj)
{
}
void    TSpacePartition::AddDynamicObject2D(TObject2D* pObj)
{
}

TNode* TSpacePartition::FindNode(TNode* pNode, TObject* pObj)
{    
    return nullptr;
}
TNode2D* TSpacePartition::FindNode(TNode2D* pNode, TObject2D* pObj)
{
    return nullptr;
}
std::vector<TObject*> TSpacePartition::CollisionQuery(TObject* pObj)
{
    std::vector<TObject*> list;    
    return list;
};
std::vector<TObject2D*> TSpacePartition::CollisionQuery(TObject2D* pObj)
{
    std::vector<TObject2D*> list;
    return list;
};
TSpacePartition::TSpacePartition()
{

}
TSpacePartition::~TSpacePartition()
{
}