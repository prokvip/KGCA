#pragma once
#include "TSpaceDivision.h"

class TBspRender : public TActor
{
public:
	void		RenderLeafBoungingBox(ID3D11DeviceContext*		pContext, TCamera* pCamera, TNode* pNode );
	void		RenderNode(ID3D11DeviceContext*		pContext, TNode* pNode );
	void		RecursionRender(ID3D11DeviceContext*		pContext, TNode* pNode );
public:
	TBspRender(void);
	virtual ~TBspRender(void);
};
