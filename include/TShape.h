#pragma once
#include "TModel.h"
class TShape : public TModel
{
};
class TPlaneShape : public TShape
{
public:
    bool CreateVertexData() override;
    bool CreateIndexData() override;
};
class TBoxShape : public TShape
{
};
